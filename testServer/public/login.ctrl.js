function tryLogin() {
    document.getElementById("loginbut").disabled = true;
    var xhr = new XMLHttpRequest();
    if (document.getElementById('username').value != "" && document.getElementById('password').value != "") {
        sendDat = { 'uname': document.getElementById('username').value, 'password': document.getElementById('password').value }
        xhr.open('POST', "/login", true);
        xhr.send(JSON.stringify(sendDat));
        function processRequest(e) {
            if (xhr.readyState == 4 && xhr.status == 200) {
                var response = JSON.parse(xhr.responseText);
                console.log(response);
                window.localStorage.token = response.val;
                window.localStorage.uname = document.getElementById('username').value;
                if (response.val != null) {
                    window.location.replace("/dash");
                }
                else {
                    //error message + enable button
                    document.getElementById("loginbut").disabled = false;
                }
            }
        }
        xhr.addEventListener("readystatechange", processRequest, false);
    }
}