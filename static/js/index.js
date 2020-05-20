$(function(){
    let firstActive = 0;
    let params = new URLSearchParams(window.location.hash.substr(1));
    let token = params.get('access_token');
    let state = params.get('state');
    let paths = [];
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
        let strPaths = '';
        paths.forEach(function(path){
            strPaths += path[0] + '=' + path[1] + '\n';
        });
        let blob = new Blob(['[Dropbox]\nToken=' + localStorage.getItem('dropboxToken') + '\n' + '[Paths]\n' + strPaths], {type: "application/octet-stream;charset=utf-8"});
        const fileStream = streamSaver.createWriteStream('3DSync.ini', {
            size: blob.size
        });
        const readableStream = blob.stream();
        if (window.WritableStream && readableStream.pipeTo) {
            return readableStream.pipeTo(fileStream)
              .then(() => console.log('done writing'));
        }
        window.writer = fileStream.getWriter();
        const reader = readableStream.getReader();
        const pump = () => reader.read()
          .then(res => res.done
            ? writer.close()
            : writer.write(res.value).then(pump));
        pump();
    });

    $('#add-custom-path').on('click', function(e){
        e.preventDefault();
        let id = Date.now();
        let $input = $('<div class="row">' +
          '<div class="input-field col s3"><input id="' + id + '-n" class="white-text" type="text"><label for="' + id + '-n" class="white-text">Name</label><span class="helper-text" data-error="Invalid name"></span></div>' +
          '<div class="input-field col s7"><input id="' + id + '" class="white-text path-custom" type="text"><label for="' + id + '" class="white-text">Path</label><span class="helper-text" data-error="Invalid path"></span></div>' +
          '<div class="col s2"><a href="#" class="btn-floating waves-effect waves-light red remove-custom-path"><i class="material-icons">remove</i></a></div></div>');
        $input.find('.remove-custom-path').on('click', function(e){
            e.preventDefault();
            $(this).parent().parent().remove();
        });
        $(this).before($input);
    });

    $('#folders-confirm').on('click', function(e){
        e.preventDefault();
        paths = [];
        let error = false;
        $('#paths-presets input:checked, #paths-custom input.path-custom').each(function(){
            let $this = $(this);
            if($this.hasClass('path-custom')){
                let path = $this.val();
                let regex = /((?:[^/]*\/)*)(.*)/.exec(path);
                if(regex[1] === ""){
                    error = true;
                    $this.addClass('invalid');
                } else {
                    $this.removeClass('invalid');
                    let $name = $('#' + $this.attr('id') + '-n');
                    if($name.val() === ''){
                        error = true;
                        $name.addClass('invalid');
                    } else {
                        $name.removeClass('invalid');
                        paths.push([$name.val(), regex[1]]);
                    }
                }
            } else {
                paths.push([$this.next().text(), $this.data('path')]);
            }
        });
        if(error === false){
            stepperInstace.nextStep();
        }
    });
});
