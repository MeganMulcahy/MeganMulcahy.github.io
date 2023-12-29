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
const navLinks = document.querySelectorAll('.nav-item')
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

function getMouseCoords(e) {
    var e = e || window.event;
    document.getElementById('msg').innerHTML =
      e.clientX + ', ' + (e.clientY + window.scrollY) + '<br>' + e.screenX + ', ' + e.screenY;
  }
  
  var followCursor = (function () {
    var s = document.querySelector('.object');
    var delay = 50; // Adjust the delay time in milliseconds
    var speed = 0.02; // Adjust the speed factor
    var targetX = 0;
    var targetY = 0;
    var currentX = 0;
    var currentY = 0;
    var maxX = window.innerWidth - s.clientWidth; // Max X within viewport
    var maxY = window.innerHeight - s.clientHeight; // Max Y within viewport
  
    return {
      init: function () {
        document.body.appendChild(s);
        // Start the animation loop
        requestAnimationFrame(this.animate.bind(this));
      },
  
      run: function (e) {
        // Update the target position when the mouse is moving
        targetX = Math.min(e.clientX - 5, maxX); // Restrict within maxX
        targetY = Math.min(e.clientY - 5 + window.scrollY, maxY); // Restrict within maxY
      },
  
      animate: function () {
        // Calculate the distance to the target mouse position
        currentX += (targetX - currentX) * speed;
        currentY += (targetY - currentY) * speed;
  
        // Update the object's position incrementally
        s.style.left = currentX + 'px';
        s.style.top = currentY + 'px';
  
        // Continue the animation loop
        requestAnimationFrame(this.animate.bind(this));
      }
    };
  })();
  
  window.onload = function () {
    followCursor.init();
    document.body.onmousemove = followCursor.run.bind(followCursor);
    window.addEventListener('scroll', function () {
      // Update the target position when scrolling
      followCursor.run.bind(followCursor)({ clientX: targetX + 5, clientY: targetY + 5 });
      followCursor.animate.bind(followCursor)();
    });
  };
  