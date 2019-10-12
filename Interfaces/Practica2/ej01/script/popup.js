var modal = document.querySelector(".modal");
var trigger = document.getElementsByClassName("trigger");
var closeButton = document.getElementsByClassName(".close-button");
var inner = document.getElementsByClassName("inner");
var card = document.getElementsByClassName("card");
var column = document.getElementsByClassName("column");
var remove = document.getElementsByClassName("close");
var confirmation = document.querySelector(".confirmation");
var cancel = document.querySelector(".calcel");
var add = document.querySelector(".add-task-btn");
var addition = document.querySelector(".addition");

//Todo el contenido en un array para que sea dinámico y poner añadir/borrar
var cards = [  
    [
        "Tasks to Do",
        [
            [
                1,
                "images/office.jpg",
                "Daily standup meeting",
                "26-09-2019"
            ],
            [
                2,
                "images/hire.jpg",
                "Hire new UI/UX developer",
                "10-12-2019"
            ]
        ]
    ],
    [
        "In progress",
        [
            [
                1,
                "images/fsf.jpg",
                "Send FSF new proposal",
                "26-11-2019"
            ],
        ]
    ],
    [
        "Finished",
        [
            [
                1,
                "images/office.jpg",
                "Meeting with CEO",
                "25-09-2019"
            ],
            [
                2,
                "images/brainstorm.png",
                "Brainstorming for new projects",
                "21-09-2019"
            ],
            [
                3,
                "images/coffe.jpg",
                "Buy new coffe machine",
                "15-09-2019"
            ],
            [
                4,
                "images/office.jpg",
                "Project review",
                "10-09-2019"
            ]
        ]
    ]
];

//Añadimos 1 a todo para obviar la columna auxiliar
$(function(){
    var it = 1;
    for(i=0; i<cards.length; i++){
        //Creamos una columna
        $('.column:first').clone(true).appendTo('.row');
        //Le ponemos el título correspondiente
        document.getElementsByClassName("card-title")[i+1].innerHTML = cards[i][0];
        for(j=0; j<cards[i][1].length; j++){
            //La primera ya existe, por lo que no la duplicamos
            if(j!=0){
                $('.inner:last').clone(true).appendTo('.card:last');
            }
            //Añadimos el resto del contenido
            document.getElementsByClassName("card-image")[it].src=cards[i][1][j][1];
            document.getElementsByClassName("card-text")[it].innerHTML = cards[i][1][j][2];
            document.getElementsByClassName("card-date")[it].innerHTML = cards[i][1][j][3];
            it++;
        }
    }
//Borramos la columna auxiliar
    $('.column:first').remove();
});

//Borrar la tarjeta en la x
for (var i=0; i < remove.length; i++) {
    remove[i].onclick = function() {
        modal.classList.toggle("show-modal");
    };
}

function popup() {
    modal.classList.toggle("show-modal");
}

function windowOnClick(event) {
    if (event.target === modal) {
        popup();
    }
}

//Abrir el popup desde cualquier tarjeta
for (var i=0; i < trigger.length; i++) {
    trigger[i].onclick = function popup() {
        modal.classList.toggle("show-modal");
    };
}

//Cerrar el popup en la x
for (var i=0; i < closeButton.length; i++) {
    closeButton[i].onclick = function popup() {
        modal.classList.toggle("show-modal");
    };
}
//Para clicar fuera del pop up
window.addEventListener("click", windowOnClick);

//Abrir el popup desde cualquier x
for (var i=0; i < remove.length; i++) {
    remove[i].onclick = function() {
        confirmation.classList.toggle("show-confirmation");
    };
}

//Cerrar el popup en cancel
for (var i=0; i < cancel.length; i++) {
    cancel[i].onclick = function() {
        confirmation.classList.toggle("show-confirmation");
    };
}

function remove(elem){

}

document.getElementById('confirm').addEventListener('click', remove());

//Abrir el popup de adición desde cualquier add task
for (var i=0; i < add.length; i++) {
    add[i].onclick = function() {
        addition.classList.toggle("show-addition");
    };
}

//Cerrar el popup en cancel
for (var i=0; i < cancel.length; i++) {
    cancel[i].onclick = function() {
        addition.classList.toggle("show-addition");
    };
}