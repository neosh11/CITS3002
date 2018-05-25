function updateStats(last, correct, tries) {
    if (tries > 0) {
        document.getElementById("tries").innerHTML = tries;
        var res = document.getElementById("status");
        if (correct) {
            res.innerText = "Correct Answer";
            res.style.color = "green";
            document.getElementById("mark").innerHTML = `Mark: ${4 - tries}`;
        }
        else {
            res.innerText = "Wrong Answer";
            res.style.color = "red";
            document.getElementById("mark").innerHTML = `Mark: ${0}`;
        }
    }
    else {
        document.getElementById("tries").innerHTML = 0;
        document.getElementById("status").innerHTML = "";
        document.getElementById("status").style.color = null;
        document.getElementById("mark").innerHTML = "";        
    }
}

function loadPage() {
    var xhr = new XMLHttpRequest();
    sendDat = { 'uname': window.localStorage.uname, 'token': window.localStorage.token }
    xhr.open('POST', "/getPQuestionData", true);
    xhr.send(JSON.stringify(sendDat));

    function processRequest(e) {
        if (xhr.readyState == 4 && xhr.status == 200) {
            var response = JSON.parse(xhr.responseText);
            console.log(response);
            if (response.question) {
                document.getElementById("Question").innerHTML = response.question;
                document.getElementById("function").innerHTML = response.function;

                document.getElementById("input").onkeydown = function (e) {
                    if (e.keyCode == 9 || e.which == 9) {
                        e.preventDefault();
                        var s = this.selectionStart;
                        this.value = this.value.substring(0, this.selectionStart) + "\t" + this.value.substring(this.selectionEnd);
                        this.selectionEnd = s + 1;
                    }
                }
                document.getElementById("input").value = response.ans;
                updateStats(response.ans, response.correct, response.tries)
            }
            else {
                window.location.replace("/");
            }
        }
    }
    xhr.addEventListener("readystatechange", processRequest, false);
}

function submit() {
    var xhr2 = new XMLHttpRequest();
    sendDat = { 'uname': window.localStorage.uname, 'token': window.localStorage.token, 'code': document.getElementById("input").value}
    xhr2.open('POST', "/markPQuestion", true);
    xhr2.send(JSON.stringify(sendDat));
    function processRequestSubmit(e) {
        if (xhr2.readyState == 4 && xhr2.status == 200) {
            console.log(xhr2.responseText)
            var response = JSON.parse(xhr2.responseText);
            if (response.tries) {
                updateStats(response.ans, response.correct, response.tries)
                //highlight last attempt
                //print number of attemps
            }
            else {
                //error message
            }
        }
    }
    xhr2.addEventListener("readystatechange", processRequestSubmit, false);
}

function moveQ(val) {
    var xhr2 = new XMLHttpRequest();
    sendDat = { 'uname': window.localStorage.uname, 'token': window.localStorage.token, 'val': val }
    xhr2.open('POST', "/movePQuestion", true);
    xhr2.send(JSON.stringify(sendDat));
    function processRequestSubmit(e) {
        if (xhr2.readyState == 4 && xhr2.status == 200) {
            var response = JSON.parse(xhr2.responseText);
            console.log(response);
            if (response.status == "success") {
                loadPage();
            }
            else {
                //failure
            }
        }
        else {
            //error
        }
    }
    xhr2.addEventListener("readystatechange", processRequestSubmit, false);
}


// Init page
loadPage();