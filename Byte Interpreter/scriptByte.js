// script.js

document.getElementById('toggleButton').addEventListener('click', function() {
    var hiddenSection = document.getElementById('hiddenSection');
    if (hiddenSection.style.display === 'none') {
        hiddenSection.style.display = 'block'; // Show the hidden section
    } else {
        hiddenSection.style.display = 'none'; // Hide the section
    }
  });
  
  document.addEventListener('click', function(event) {
    var clickedElement = event.target;
  
    // Check if the clicked element has the id "nestedToggleButton"
    if (clickedElement.id === 'nestedToggleButton') {
      var nestedHiddenSection = clickedElement.nextElementSibling;
  
      if (nestedHiddenSection.style.display === 'none') {
        nestedHiddenSection.style.display = 'block'; // Show the hidden section
      } else {
        nestedHiddenSection.style.display = 'none'; // Hide the section
      }
    }
  });
  
  