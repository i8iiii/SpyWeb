const statusText = document.getElementById("statusText");
const logBox = document.getElementById("logBox");

function log(msg) {
  logBox.innerHTML += `<div>${msg}</div>`;
  logBox.scrollTop = logBox.scrollHeight;
}

document.getElementById("hostBtn").addEventListener("click", () => {
  statusText.textContent = "Hosting...";
  log("Waiting for a player to join...");
  setTimeout(() => {
    statusText.textContent = "Matched!";
    log("Player joined successfully!");
  }, 3000);
});

document.getElementById("joinBtn").addEventListener("click", () => {
  statusText.textContent = "Searching...";
  log("Looking for host...");
  setTimeout(() => {
    statusText.textContent = "Matched!";
    log("Connected to host!");
  }, 3000);
});
