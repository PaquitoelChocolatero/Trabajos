var modal = document.querySelector(".modal");
var trigger = document.getElementsByClassName("trigger");
var closeButton = document.getElementsByClassName(".close-button");

function popup() {
    modal.classList.toggle("show-modal");
}

function windowOnClick(event) {
    if (event.target === modal) {
        popup();
    }
}

for (var i=0; i < trigger.length; i++) {
    console.log(trigger[i]);
    trigger[i].onclick = function popup() {
        modal.classList.toggle("show-modal");
    };
}

for (var i=0; i < closeButton.length; i++) {
    closeButton[i].onclick = function popup() {
        modal.classList.toggle("show-modal");
    };
}

window.addEventListener("click", windowOnClick);
