$(document).ready(function(){
    var modal = document.querySelector(".modal");
    var confirmation = document.querySelector(".confirmation");
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
            $('.column:first').clone(true, true).appendTo('.row');
            //Le ponemos el título correspondiente
            document.getElementsByClassName("card-title")[i+1].innerHTML = cards[i][0];
            for(j=0; j<cards[i][1].length; j++){
                //La primera ya existe, por lo que no la duplicamos
                if(j!=0){
                    $('.inner:last').clone(true, true).appendTo('.card:last');
                }
                //Añadimos el resto del contenido
                document.getElementsByClassName("card-image")[it].src=cards[i][1][j][1];
                document.getElementsByClassName("card-text")[it].innerHTML = cards[i][1][j][2];
                document.getElementsByClassName("card-date")[it].innerHTML = cards[i][1][j][3];
                it++;
            }
        }
    //Borramos la columna auxiliar
        $('.column:first').hide();
    });

    //Borrar la tarjeta en la x
    $('div.inner').on('click', 'div.close', function() {
        modal.classList.toggle("show-modal");
    });

    function popup() {
        modal.classList.toggle("show-modal");
    };
    
    function popup2() {
        confirmation.classList.toggle("show-confirmation");
    };
    
    function popup3() {
        addition.classList.toggle("show-addition");
    };

    function windowOnClick(event) {
        if (event.target === modal) {
            popup();
        }
        else if (event.target === confirmation) {
            popup2();
        }
        else if (event.target === addition) {
            popup3();
        }
    }

    //Abrir el popup desde cualquier tarjeta
    $('div.inner').on('click', 'div.trigger', function() {
        modal.classList.toggle("show-modal");
    });

    //Cerrar el popup en la x
    $('div.modal').on('click', 'div.close-button', function() {
        modal.classList.toggle("show-modal");
    });

    //Para clicar fuera del pop up
    window.addEventListener("click", windowOnClick);

    var current_task;
    //Abrir el popup desde cualquier x
    $('div.inner').on('click', 'button.close', function() {
        current_task = $(this).parent();
        confirmation.classList.toggle("show-confirmation");
    });

    //Cerrar el popup en cancel
    $('div.confirmation').on('click', '#cancel', function() {
        confirmation.classList.toggle("show-confirmation");
    });

    //En confirm cerramos el popup y borramos la tarjeta
    $('div.confirmation').on('click', '#confirm', function() {
        //cards.splice(cards.indexOf($(current_task).find('.card-text')), 1);
        cards[0][1].splice(1, 1);
        alert(cards);
        confirmation.classList.toggle("show-confirmation");
    });

    //Re-render page (?)
    $(function(){
        $('.column:first').show();
        $('.column:not(:first)').remove();
        var it = 1;
        for(i=0; i<cards.length; i++){
            //Creamos una columna
            $('.column:first').clone(true, true).appendTo('.row');
            //Le ponemos el título correspondiente
            document.getElementsByClassName("card-title")[i+1].innerHTML = cards[i][0];
            for(j=0; j<cards[i][1].length; j++){
                //La primera ya existe, por lo que no la duplicamos
                if(j!=0){
                    $('.inner:last').clone(true, true).appendTo('.card:last');
                }
                //Añadimos el resto del contenido
                document.getElementsByClassName("card-image")[it].src=cards[i][1][j][1];
                document.getElementsByClassName("card-text")[it].innerHTML = cards[i][1][j][2];
                document.getElementsByClassName("card-date")[it].innerHTML = cards[i][1][j][3];
                it++;
            }
        }
    //Borramos la columna auxiliar
        $('.column:first').hide();
    });

    //Abrir el popup de adición desde cualquier add task
    $('button.btn').on('click', function() {
        addition.classList.toggle("show-addition");
    });

    //Cerrar el popup en cancel
    $('div.addition').on('click', '#cancel', function() {
        addition.classList.toggle("show-addition");
    });

    //En confirm cerramos el popup y borramos la tarjeta
    $('div.addition').on('click', '#confirm', function() {
        addition.classList.toggle("show-addition");
    });

    //Botón like con contador
    $(".like_button").on("click", function() {
        var $like_count = $(this).parent().find('.like_count');
        $count.html($like_count.html() + 1);
    });


    //Botón kebab menu
    function myFunction() {
        document.getElementById("myDropdown").classList.toggle("show");
    }

    // Close the dropdown if the user clicks outside of it
    window.onclick = function(event) {
        if (!event.target.matches('.dropbtn')) {
            var dropdowns = document.getElementsByClassName("dropdown-content");
            var i;
            for (i = 0; i < dropdowns.length; i++) {
                var openDropdown = dropdowns[i];
                if (openDropdown.classList.contains('show')) {
                    openDropdown.classList.remove('show');
                }
            }
        }
    }
});
