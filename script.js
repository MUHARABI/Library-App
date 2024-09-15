// Initialize library
let library = [];

// Firebase configuration
const firebaseConfig = {
    apiKey: "your-api-key",
    authDomain: "your-auth-domain",
    projectId: "your-project-id",
    storageBucket: "your-storage-bucket",
    messagingSenderId: "your-messaging-sender-id",
    appId: "your-app-id",
    databaseURL: "your-database-url"
};

// Initialize Firebase
const app = firebase.initializeApp(firebaseConfig);
const db = firebase.database();

// Load library from Firebase
function loadFromFirebase() {
    db.ref('library').on('value', (snapshot) => {
        library = snapshot.val() || [];
        displayLibraryEntries();
    });
}

// Save library to Firebase
function saveToFirebase() {
    db.ref('library').set(library);
}

// Display library entries
function displayLibraryEntries() {
    const responseDiv = document.getElementById('response');
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
document.getElementById('addForm').addEventListener('submit', (event) => {
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
    saveToFirebase();
    displayLibraryEntries();
    showForm('');
});

// Remove book from library
document.getElementById('removeForm').addEventListener('submit', (event) => {
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

    saveToFirebase();
    displayLibraryEntries();
    showForm('');
});

// Search for a book in the library
document.getElementById('searchForm').addEventListener('submit', (event) => {
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

    const responseDiv = document.getElementById('response');
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

// Checkout book from library
document.getElementById('checkoutForm').addEventListener('submit', (event) => {
    event.preventDefault();
    const checkoutTitle = document.getElementById('checkoutTitle').value;

    let bookFound = false;
    library = library.map(book => {
        if (book.title === checkoutTitle && book.status === 'In Library') {
            book.status = 'Checked Out';
            bookFound = true;
        }
        return book;
    });

    const responseDiv = document.getElementById('response');
    if (bookFound) {
        saveToFirebase();
        displayLibraryEntries();
        responseDiv.innerHTML = '<p>Book successfully checked out.</p>';
    } else {
        responseDiv.innerHTML = '<p>Book not found or already checked out.</p>';
    }

    showForm('');
});

// Clear library
document.getElementById('clearBtn').addEventListener('click', () => {
    if (confirm('Are you sure you want to clear the entire library?')) {
        library = [];
        saveToFirebase();
        displayLibraryEntries();
    }
});

// Utility to show/hide forms
function showForm(formId) {
    document.querySelectorAll('main div').forEach(form => form.style.display = 'none');
    if (formId) {
        document.getElementById(formId).style.display = 'block';
    }
}

document.getElementById('addBtn').addEventListener('click', () => showForm('addItemForm'));
document.getElementById('removeBtn').addEventListener('click', () => showForm('removeItemForm'));
document.getElementById('checkoutBtn').addEventListener('click', () => showForm('checkoutItemForm'));
document.getElementById('searchBtn').addEventListener('click', () => showForm('searchItemForm'));

// Load library on page load
document.addEventListener('DOMContentLoaded', loadFromFirebase);
