import json
import subprocess
import os
from pathlib import Path
import ast

def test_code_snippet(code_snippet, executable_path):
    """Test a code snippet using main.exe."""
    temp_file = None
    try:
        temp_file = "temp_test.py"
        tree = ast.parse(code_snippet)
        unparsed_code = ast.unparse(tree)
        with open(temp_file, "w", encoding='utf-8') as f:
            f.write(unparsed_code)
        
        result = subprocess.run([executable_path, temp_file], 
                              capture_output=True, 
                              text=True,
                              encoding='utf-8',
                              errors='replace')
        
        return result.returncode == 0
        
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
    input_file = Path("dataset.jsonl")
    output_file = Path("valid_codes.jsonl")
    
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
    
    valid_entries = []
    total_entries = 0
    
    try:
        with open(input_file, 'r', encoding='utf-8') as f:
            for line in f:
                try:
                    entry = json.loads(line.strip())
                    if "correct_code" not in entry:
                        continue
                        
                    code_snippet = entry["correct_code"]
                    total_entries += 1
                        
                    if test_code_snippet(code_snippet, executable_path):
                        valid_entries.append(entry)
                        # print(f"✅ Código válido encontrado en entrada {total_entries}")
                    # else:
                        # print(f"❌ Código inválido en entrada {total_entries}")
                        
                except json.JSONDecodeError:
                    print(f"Error parsing JSON line: {line[:50]}...")
                    continue
                
        with open(output_file, 'w', encoding='utf-8') as f:
            for entry in valid_entries:
                f.write(json.dumps(entry, ensure_ascii=False) + '\n')
                
        print(f"\nProcesamiento completado!")
        print(f"Total de entradas procesadas: {total_entries}")
        print(f"Entradas válidas: {len(valid_entries)}")
        print(f"Resultados guardados en: {output_file}")
        
    except Exception as e:
        print(f"Error procesando dataset: {e}")

if __name__ == "__main__":
    process_dataset() 