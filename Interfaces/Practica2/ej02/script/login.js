$(document).ready(function(){
    
    $('#forgotten').on('click', function(){
        alert('   Too bad \n ¯\\_(•_•)_/¯');
    })

    $('div.options').on('click', '.SI', function(){
        $('div.Sign-Up').remove();
        $('div.Sign-In').remove();
        $('.central').append(`
        <div class='Sign-In'>
            <form id='info'>
                <input id="user" type="text" placeholder="Email" required>
                <br />
                <br />
                <input id="pass" type="password" maxlength='8' pattern='[A-Za-z0-9]+' placeholder="Password" required>
                <button class='go'>Sign In</button>
                <a id='forgotten'>Forgot your password?</a>
            </form>
        </div>        
        `);
    })

    $('div.options').on('click', '.SU', function(){
        $('div.Sign-In').remove();
        $('div.Sign-Up').remove();
        $('.central').append(`
        <div class='Sign-Up'>
            <form id='info'>
                <input id="user" type="text" placeholder="Email" required>
                <br />
                <br />
                <input id="pass" type="password" maxlength='8' pattern='[A-Za-z0-9]+' placeholder="Password" required>
                <br />
                <br />
                <div class='name'>
                    <input type="text" placeholder="Name" required>
                    <input type="text" placeholder="Last name" required>
                </div>
                <br />
                <br />
                <button class='go'>Sign Up</button>
            </form>
        </div>        
        `);
    })

});