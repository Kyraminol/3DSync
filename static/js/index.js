$(function(){
    let firstActive = 0;
    let params = new URLSearchParams(window.location.hash.substr(1));
    let token = params.get('access_token');
    let state = params.get('state');
    if(token !== null && state !== null){
        if(state === localStorage.getItem('token')){
            localStorage.setItem('dropboxToken', token);
            firstActive = 1;
        }
    }
    localStorage.removeItem('token');

    let stepperInstace = new MStepper(document.querySelector('.stepper'), {
        firstActive: firstActive,
        autoFormCreation: false,
        stepTitleNavigation: false,
    });

    $('#dropbox-login').on('click', function(e){
        e.preventDefault();
        let token = [...Array(100)].map(i=>(~~(Math.random()*36)).toString(36)).join('');
        localStorage.setItem('token', token);
        window.location.href = "https://www.dropbox.com/oauth2/authorize?client_id=3x8ipjhtplvcoba&response_type=token&redirect_uri=https://kyraminol.github.io/3DSync/&state=" + token;
    });

    $('#download-config').on('click', function(e){
        e.preventDefault();
        let blob = new Blob(["[Dropbox]\nToken=" + localStorage.getItem('dropboxToken') + '\n'], {type: "application/octet-stream;charset=utf-8"});
        saveAs(blob, "3DSync.ini");
    });
});
