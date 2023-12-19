
  function loadContent(tabId) {
    fetch(tabId.toLowerCase() + '.html')
      .then(response => response.text())
      .then(html => {
        document.getElementById('content-container').innerHTML = html;
      })
      .catch(error => console.error('Error loading content:', error));
  }
  
  // Initial load for the Projects tab
  loadContent('Projects');
  