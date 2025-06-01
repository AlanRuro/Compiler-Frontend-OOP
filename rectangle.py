class Rectangulo:
    def init(self, base, altura):
        self.base = base
        self.altura = altura

    def calcular_area(self):
        return self.base * self.alutra  # Error: 'alutra' está mal escrito

    def calcular_perimetro(self):
        return 2 * (self.base + self.altura)

    def str(self):
        return f"Rectángulo de base {self.base} y altura {self.altura}"

rect = Rectangulo(5, 10)
print(rect)
print("Área:", rect.calcular_area())
print("Perímetro:", rect.calcular_perimetro())
