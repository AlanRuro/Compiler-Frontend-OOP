import json
import subprocess
import os
from pathlib import Path
import ast
import re

def has_invalid_escape_sequences(code):
    """Check if the code contains invalid escape sequences."""
    invalid_escapes = [
        r'\\[^\\\'"abfnrtv0]',  # Matches invalid escape sequences
        r'\\[^\\\'"abfnrtv0]',  # Matches invalid escape sequences in raw strings
    ]
    for pattern in invalid_escapes:
        if re.search(pattern, code):
            return True
    return False

def extract_code_snippet(prompt):
    """Extract the code snippet from the prompt string."""
    # Find the line after "##Buggy Code:"
    lines = prompt.split('\n')
    for i, line in enumerate(lines):
        if "##Buggy Code:" in line and i + 1 < len(lines):
            code = lines[i + 1]
            # Filter out code with invalid escape sequences
            if has_invalid_escape_sequences(code):
                return None
            return code
    return None

def test_code_snippet(code_snippet, executable_path):
    """Test a code snippet using main.exe."""
    if not code_snippet or has_invalid_escape_sequences(code_snippet):
        return False

    temp_file = None
    try:
        # Create a temporary file with the code snippet
        tree = ast.parse(code_snippet)
        unparsed_code = ast.unparse(tree)

        temp_file = "temp_test.py"
        with open(temp_file, "w", encoding='utf-8') as f:
            f.write(unparsed_code)
        
        # Run main.exe with the temporary file
        result = subprocess.run([executable_path, temp_file], 
                              capture_output=True, 
                              text=True,
                              encoding='utf-8',
                              errors='replace')
        
        # Check for specific error patterns
        if result.stderr:
            if "SyntaxWarning: invalid escape sequence" in result.stderr:
                return False
            if "SyntaxError" in result.stderr:
                return False
        
        # Return True if compilation was successful (no error output)
        success = result.returncode == 0 and not result.stderr
        return success
        
    except SyntaxError:
        return False
    except Exception as e:
        print(f"Error testing code snippet: {e}")
        return False
    finally:
        if temp_file and os.path.exists(temp_file):
            try:
                os.remove(temp_file)
            except Exception as e:
                print(f"Error al eliminar archivo temporal: {e}")

def process_dataset():
    # Read the input dataset
    input_file = Path("small_datset.jsonl")
    output_file = Path("successful_compilations.json")
    
    if not input_file.exists():
        print(f"Error: No se encontró el archivo {input_file}")
        return
    
    try:
        executable_path = Path("../build/bin/main.exe").absolute()
        if not executable_path.exists():
            print(f"Error: No se encontró el ejecutable en {executable_path}")
            return
        print(f"Usando ejecutable en: {executable_path}")
    except Exception as e:
        print(f"Error: {e}")
        print("Por favor, compila el proyecto primero")
        return
    
    successful_entries = []
    total_entries = 0
    filtered_entries = 0
    
    try:
        with open(input_file, 'r', encoding='utf-8') as f:
            for line in f:
                try:
                    entry = json.loads(line.strip())
                    code_snippet = extract_code_snippet(entry["prompt"])
                    
                    if code_snippet:
                        total_entries += 1
                        if has_invalid_escape_sequences(code_snippet):
                            filtered_entries += 1
                            continue
                            
                        if test_code_snippet(code_snippet, executable_path):
                            successful_entries.append(entry)
                        
                except json.JSONDecodeError:
                    print(f"Error parsing JSON line: {line[:50]}...")
                    continue
                
        # Write successful entries to output file
        with open(output_file, 'w', encoding='utf-8') as f:
            for entry in successful_entries:
                f.write(json.dumps(entry, ensure_ascii=False) + '\n')
                
        print(f"\nProcesamiento completado!")
        print(f"Total de entradas procesadas: {total_entries}")
        print(f"Entradas filtradas (secuencias de escape inválidas): {filtered_entries}")
        print(f"Entradas exitosas: {len(successful_entries)}")
        print(f"Resultados guardados en: {output_file}")
        
    except Exception as e:
        print(f"Error procesando dataset: {e}")

if __name__ == "__main__":
    process_dataset() 