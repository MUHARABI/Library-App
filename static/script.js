// Initialize library
let library = [];

// Get DOM elements
const addForm = document.getElementById('addForm');
const removeForm = document.getElementById('removeForm');
const searchForm = document.getElementById('searchForm');
const checkoutForm = document.getElementById('checkoutForm');
const responseDiv = document.getElementById('response');

// Load library from local storage
function loadFromLocalStorage() {
    const storedLibrary = localStorage.getItem('library');
    if (storedLibrary) {
        library = JSON.parse(storedLibrary);
    } else {
        library = [];
    }
    displayLibraryEntries();
}

// Save library to local storage
function saveToLocalStorage() {
    localStorage.setItem('library', JSON.stringify(library));
}

// Display library entries
function displayLibraryEntries() {
    responseDiv.innerHTML = '<h2>Library Inventory:</h2>';
    if (library.length === 0) {
        responseDiv.innerHTML += '<p>No books in the library.</p>';
    } else {
        const bookEntriesDiv = document.createElement('div');
        bookEntriesDiv.classList.add('book-entries');
        
        library.forEach(book => {
            const bookEntryDiv = document.createElement('div');
            bookEntryDiv.classList.add('book-entry');
            
            const bookTitle = document.createElement('h3');
            bookTitle.textContent = `Title: ${book.title}`;
            
            const bookIsbn = document.createElement('p');
            bookIsbn.textContent = `ISBN: ${book.isbn}`;
            
            const bookStatus = document.createElement('p');
            bookStatus.textContent = `Status: ${book.status}`;
            
            bookEntryDiv.appendChild(bookTitle);
            bookEntryDiv.appendChild(bookIsbn);
            bookEntryDiv.appendChild(bookStatus);
            bookEntriesDiv.appendChild(bookEntryDiv);
        });

        responseDiv.appendChild(bookEntriesDiv);
    }
}

// Add book to library
addForm.addEventListener('submit', (event) => {
    event.preventDefault();
    const bookTitle = document.getElementById('bookTitle').value;
    const isbn = document.getElementById('isbn').value;

    // Validate the ISBN format
    if (isbn.length !== 17 || !/^(\d{3}-\d{1}-\d{2}-\d{6}-\d{1})$/.test(isbn)) {
        alert('Invalid ISBN format.');
        return;
    }

    // Add book to library
    library.push({ title: bookTitle, isbn, status: 'In Library' });
    saveToLocalStorage();
    displayLibraryEntries();
    showForm(''); 
});

// Remove book from library
removeForm.addEventListener('submit', (event) => {
    event.preventDefault();
    const removeBy = document.getElementById('removeBy').value;
    const removeValue = document.getElementById('removeValue').value;

    library = library.filter(book => {
        if (removeBy === 'name' && book.title === removeValue) {
            return false;
        }
        if (removeBy === 'isbn' && book.isbn === removeValue) {
            return false;
        }
        return true;
    });

    saveToLocalStorage();
    displayLibraryEntries();
    showForm(''); // Hide form after removing
});

// Search for a book in the library
searchForm.addEventListener('submit', (event) => {
    event.preventDefault();
    const searchBy = document.getElementById('searchBy').value;
    const searchValue = document.getElementById('searchValue').value;

    const searchResults = library.filter(book => {
        if (searchBy === 'name' && book.title === searchValue) {
            return true;
        }
        if (searchBy === 'isbn' && book.isbn === searchValue) {
            return true;
        }
        return false;
    });

    responseDiv.innerHTML = '<h2>Search Results:</h2>';
    if (searchResults.length === 0) {
        responseDiv.innerHTML += '<p>No matching books found.</p>';
    } else {
        const ul = document.createElement('ul');
        searchResults.forEach(book => {
            const li = document.createElement('li');
            li.textContent = `Title: ${book.title}, ISBN: ${book.isbn}, Status: ${book.status}`;
            ul.appendChild(li);
        });
        responseDiv.appendChild(ul);
    }
});

// Clear library
document.getElementById('clearBtn').addEventListener('click', () => {
    if (confirm('Are you sure you want to clear the entire library?')) {
        library = [];
        saveToLocalStorage();
        displayLibraryEntries();
    }
});

// Function to show or hide forms
function showForm(formId) {
    document.getElementById('addItemForm').style.display = formId === 'addItemForm' ? 'block' : 'none';
    document.getElementById('removeItemForm').style.display = formId === 'removeItemForm' ? 'block' : 'none';
    document.getElementById('searchItemForm').style.display = formId === 'searchItemForm' ? 'block' : 'none';
    document.getElementById('checkoutItemForm').style.display = formId === 'checkoutItemForm' ? 'block' : 'none';
}

// Event listeners for side tabs
document.getElementById('addBtn').addEventListener('click', () => showForm('addItemForm'));
document.getElementById('removeBtn').addEventListener('click', () => showForm('removeItemForm'));
document.getElementById('searchBtn').addEventListener('click', () => showForm('searchItemForm'));
document.getElementById('checkoutBtn').addEventListener('click', () => showForm('checkoutItemForm'));

// Initialize on page load
document.addEventListener('DOMContentLoaded', loadFromLocalStorage);
