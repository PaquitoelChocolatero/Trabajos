var modal = document.querySelector(".modal");
var trigger = document.getElementsByClassName("trigger");
var closeButton = document.getElementsByClassName(".close-button");
var inner = document.getElementsByClassName("inner");
var card = document.getElementsByClassName("card");
var column = document.getElementsByClassName("column");


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
    console.log(trigger[i]);
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
                "images/brainstorm.jpg",
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

for(i=0; i<cards.length; i++){
    //Creamos una columna
    $(function(){
        $('.column:first').clone().appendTo('.row');
    })
    //Le ponemos el título correspondiente
    document.getElementById("card-title").innerHTML = cards[i][0];
    for(j=0; j<cards[i][1].length; j++){
        //La primera ya existe, por lo que no la duplicamos
        if(j!=0){
            $(function(){
                $('.inner:last').clone().appendTo('.card:last');
            })
        }
        //Añadimos el resto del contenido
        document.getElementById("card-image").src=cards[i][1][j][1];
        document.getElementById("card-text").innerHTML = cards[i][1][j][2];
        document.getElementById("card-date").innerHTML = cards[i][1][j][3];
    }
}
//Borramos la columna auxiliar
$(function(){
    $('.column:first').remove();
});

//Para clicar fuera del pop up
window.addEventListener("click", windowOnClick);
