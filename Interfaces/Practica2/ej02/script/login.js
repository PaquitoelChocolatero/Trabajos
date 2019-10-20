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
                <input id="user" type="text" placeholder="User" required>
                <br />
                <br />
                <input id="pass" type="password" maxlength='8' pattern='[A-Za-z0-9]+' placeholder="Password" required>
                <br />
                <br />
                <div class='name'>
                    <div><input type="text" placeholder="Name" required></div>
                    </t>
                    <div><input type="text" placeholder="Last name" required></div>
                </div>
                <br />
                <input id="email" type="email" placeholder="Email" required>
                <br />
                <br />
                <input id="birth" type="date" placeholder="Your birth date" required>
                <br />
                <br />
                <select id='interest'>
                    <option value="" selected>Please choose your interest... (optional)</option>
                    <option value='Being organized'>Being organized</option>
                    <option value='Computer Science'>Computer Science</option>
                    <option value='Movies'>Movies</option>
                    <option value='Work' disabled>Work</option>
                    <option value='Nothing'>Nothing at all</option>
                </select>
                <br />
                <br />
                <select id='interest'>
                    <option value="" selected>Please choose your language... (optional)</option>
                    <option value='Spanish' disabled>Spanish</option>
                    <option value='Chinese' disabled>Chinese</option>
                    <option value='English' disabled>English</option>
                    <option value='Esperanto'>Esperanto</option>
                    <option value='Russian' disabled>Russian</option>
                </select>
                <br />
                <br />
                <textarea id='interest' name="objective" cols="40" rows="2" placeholder='What do you plan to use the site for? (optional)'></textarea>
                <br />
                <br />
                <div class='checked'>
                    <input id="check" name='check' type="checkbox" value='accepted' required> 
                    <label for='check'>Confirm you haven't read our terms and conditions</label>
                </div>
                <button type="button" id="cancel">Cancel</button>
                <button type="button" id="confirm"><strong>Sign Up</strong></button>
            </form>
        </div>        
        `);
    })

});