class Book:
    def __init__(self, title, author, year):
        self.title = title
        self.author = author 
        self.year = year

    def display_info(self):
        print("Title:", self.title)
        print("Author:", self.author)
        print("Year:", self.year)

class Library:
    def __init__(self, name):
        self.name = name
        self.books = []

    def add_book(self, book):
        self.books.append(book)   

    def list_books(self):
        if len(self.books) == 0:
            print("The library is empty")
        else:
            for i in range(len(self.books)): 
                book.display_info()  

    def find_book_by_title(self, title):
        for book in self.books:
            if book.title == title:
                return book
        return None

# Código de prueba
lib = Library("Central Library")
book1 = Book("1984", "George Orwell", 1949)
book2 = Book("To Kill a Mockingbird", "Harper Lee", 1960)

lib.add_book(book1)
lib.add_book(book2)

lib.list_books()