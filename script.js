// add class navbarDark on navbar scroll
const header = document.querySelector('.navbar');
console.log(header)
window.onscroll = function() {
    const top = window.scrollY;
    if(top >=100) {
        header.classList.add('navbarDark');
    }
    else {
        header.classList.remove('navbarDark');
    }
}

// collapse navbar after click on small devices
const navLinks = document.querySelectorAll('.nav-link');
const menuToggle = document.getElementById('navbarSupportedContent')

navLinks.forEach((l) => {
    l.addEventListener('click', (event) => {
        // Your logic for handling the navigation click goes here

        // Overwrite the history entry
        history.replaceState(null, null, `#${event.target.getAttribute('href').substring(1)}`);
        
        // Toggle the menu
        new bootstrap.Collapse(menuToggle).toggle();
    });
});
