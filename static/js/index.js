$(function(){
    let url = new URL(window.location.href);
    let code = url.searchParams.get("code");
    let state = url.searchParams.get("state");
    if(code !== null && state !== null){

    }

    let stepperInstace = new MStepper(document.querySelector('.stepper'), {
        firstActive: 0,
    });

    $('#dropbox-login').on('click', function(e){
        e.preventDefault();
        let token = [...Array(100)].map(i=>(~~(Math.random()*36)).toString(36)).join('');
        localStorage.setItem('token', token);
        window.location.href = "https://www.dropbox.com/oauth2/authorize?client_id=3x8ipjhtplvcoba&response_type=code&redirect_uri=https://kyraminol.github.io/3DSync/&state=" + token;
    })
});
