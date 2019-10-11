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

//Open the pop up in every card
for (var i=0; i < trigger.length; i++) {
    console.log(trigger[i]);
    trigger[i].onclick = function popup() {
        modal.classList.toggle("show-modal");
    };
}

//Close the popup
for (var i=0; i < closeButton.length; i++) {
    closeButton[i].onclick = function popup() {
        modal.classList.toggle("show-modal");
    };
}

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
    $(function(){
        $('.column:first').clone().appendTo('.row');
    })
    document.getElementById("card-title").innerHTML = cards[i][0];
    for(j=0; j<cards[i][1].length; j++){
        if(j==0){
            document.getElementById("card-image").src=cards[i][1][j][1];
            document.getElementById("card-text").innerHTML = cards[i][1][j][2];
            document.getElementById("card-date").innerHTML = cards[i][1][j][3];
        }else{
            $(function(){
                $('.inner:last').clone().appendTo('.card:last');
            })
            document.getElementById("card-image").src=cards[i][1][j][1];
            document.getElementById("card-text").innerHTML = cards[i][1][j][2];
            document.getElementById("card-date").innerHTML = cards[i][1][j][3];
        }
    }
}
$(function(){
    $('.column:first').remove();
});

window.addEventListener("click", windowOnClick);
