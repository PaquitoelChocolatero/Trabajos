$(document).ready(function(){
    var modal = document.querySelector(".modal");
    var confirmation = document.querySelector(".confirmation");
    var addition = document.querySelector(".addition");
    var share = document.querySelector(".share");

    //Set cookie values
    if(sessionStorage.getItem('user') != null){
        $('.dropbtn-user a.active').html("<img src='images/user.png' id='userImg'></img>" + sessionStorage.getItem('user'));
        $('p#Welcome').text('Welcome ' + sessionStorage.getItem('user') + '!');
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
    
    function share() {
        share.classList.toggle("show-share");
    };

    function windowOnClick(event) {
        if (event.target === modal) popup();
        else if (event.target === confirmation) popup2();
        else if (event.target === share) share();
        else if (event.target === addition){
            //Reset border
            $('#newText').css('border', 'solid rgb(158, 157, 157) 1px');
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

    //Capture task that has been clicked
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
        current_task.remove();
        confirmation.classList.toggle("show-confirmation");
    });

    //Abrir el popup de adición desde cualquier add task
    $('button.btn').on('click', function() {
        current_task = $(this).siblings();
        addition.classList.toggle("show-addition");
    });

    //Cerrar el popup en cancel
    $('div.addition').on('click', '#cancel', function() {
        //Reset border
        $('#newText').css('border', 'solid rgb(158, 157, 157) 1px');
        addition.classList.toggle("show-addition");
    });
    
    //Dropdown
    $('div.title').on('click', 'div.dropdown', function() {
        current_task = $(this).parent().parent().parent();
        $(this).children('div.dropdown-content').toggle("show-dropdown");
    });

    $('a#SO').on('click', function() {
        $('.dropbtn-user a.active').html("<img src='images/user.png' id='userImg'></img>" + 'Guest43636');
        $('p#Welcome').text('Welcome Guest43636!');
        window.open('login.html', '_self');
    });
    
    //Archive
    $('div.title').on('click', 'a.archive', function() {
        current_task.remove();
    });
    
    //Share
    $('div.inner').on('click', 'div.share_button', function() {
        current_task = $(this).parent().parent();
        share.classList.toggle("show-share");
    });

    //Like
    $('div.inner').on('click', 'div.like_button', function() {
        /*
        if ($(this).attr('src') == 'images/like.svg'){
            $(this).parent().prepend('<input type="image" alt="like" id="lk" src="images/like-active.png" width="24" height="24" title="Show others you support this task">');
            $(this).remove();
        }else{
            $(this).parent().prepend('<input type="image" alt="like" id="lk" src="images/like.svg" width="24" height="24" title="Show others you support this task">');
            $(this).remove();
        }*/
        $(this).parent().prepend('<input type="image" alt="like" id="lk" src="images/like-active.png" width="24" height="24" title="Show others you support this task">');
        $(this).remove();
    });

    //Capturamos el día de hoy
    var today = new Date();
    var dd = String(today.getDate()).padStart(2, '0');
    var mm = String(today.getMonth() + 1).padStart(2, '0');
    var yyyy = today.getFullYear();

    today = dd + '-' + mm + '-' + yyyy;

    //En confirm cerramos el popup y borramos la tarjeta
    $('div.addition').on('click', '#confirm', function() {
        if($('#newText').val() == '' || $('#newText').val() == undefined){
            $('#newText').css('border', 'solid red 2px');
        }else{
            //Clonamos un inner
            current_task.children('.inner:last').clone(true, true).appendTo(current_task);
            //Añadimos el resto del contenido
            current_task.find('.card-text:last').html($('#newText').val());
            
            //Default value to new image
            var newImg = "images/office.jpg";
            //If image is provided set value
            if($('#newImg').val()!=""){
                newImg = $('#newImg').val();
            }
            current_task.find('.card-image:last').attr('src', newImg);
            
            current_task.find('.card-date:last').html(today);

            //Reset values
            $('#newText').val('');
            $('#newImg').val('');
            
            //Reset border
            $('#newText').css('border', 'solid rgb(158, 157, 157) 1px');

            //Salimos del popup
            addition.classList.toggle("show-addition");
        }
    });    
});
