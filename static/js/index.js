$(function(){
    let firstActive = 0;
    let url = new URL(window.location.href);
    let code = url.searchParams.get("code");
    let state = url.searchParams.get("state");
    if(code !== null && state !== null){
        if(state === localStorage.getItem('token')){
            localStorage.setItem('dropboxToken', code);
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
        window.location.href = "https://www.dropbox.com/oauth2/authorize?client_id=3x8ipjhtplvcoba&response_type=code&redirect_uri=https://kyraminol.github.io/3DSync/&state=" + token;
    });

    $('#download-config').on('click', function(e){
        e.preventDefault();
        let blob = new Blob(["[Dropbox]\nToken=" + localStorage.getItem('dropboxToken')], {type: "application/octet-stream;charset=utf-8"});
        saveAs(blob, "3DSync.ini");
    });
});
