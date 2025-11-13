const statusText = document.getElementById("statusText");
const logBox = document.getElementById("logBox");

function log(msg) {
    logBox.innerHTML += `<div>${msg}</div>`;
    logBox.scrollTop = logBox.scrollHeight;
}

// Attach event listeners to the Host and Join buttons
document.getElementById("hostBtn").addEventListener("click", () => {
    statusText.textContent = "Hosting...";
    log("Waiting for a player to join...");
    
    // Simulate successful connection after 3 seconds
    setTimeout(() => {
        // statusText.textContent is now set by onConnectionSuccessful()
        log("Player joined successfully!");
        // CALL THE SUCCESS FUNCTION HERE
        onConnectionSuccessful(); 
    }, 3000);
});

document.getElementById("joinBtn").addEventListener("click", () => {
    statusText.textContent = "Searching...";
    log("Looking for host...");
    
    // Simulate successful connection after 3 seconds
    setTimeout(() => {
        // statusText.textContent is now set by onConnectionSuccessful()
        log("Connected to host!");
        // CALL THE SUCCESS FUNCTION HERE
        onConnectionSuccessful(); 
    }, 3000);
});


//******************************************************************
// Navigation Logic
//******************************************************************

// Get the two main page containers
const matchingPage = document.getElementById('matchingPage');
const menuPage = document.getElementById('menuPage');

// Get the navigation buttons
const nextBtn = document.getElementById('nextBtn');
const backBtn = document.getElementById('backBtn');

function onConnectionSuccessful() {
    // 1. Update the status text
    statusText.textContent = 'Connected! Ready to proceed.';
    
    // 2. Enable the Next button and make it look clickable
    nextBtn.disabled = false;
    nextBtn.style.opacity = '1.0';
    nextBtn.style.pointerEvents = 'auto'; // Re-enable pointer events
}

function showMenuPage() {
    // Hide the matching page
    matchingPage.style.display = 'none';
    
    // Show the menu page
    menuPage.style.display = 'block';
}

function showMatchingPage() {
    // Show the matching page
    matchingPage.style.display = 'block';
    
    // Hide the menu page
    menuPage.style.display = 'none';
}

// Add Event Listeners for the navigation
nextBtn.addEventListener('click', showMenuPage);
backBtn.addEventListener('click', showMatchingPage);