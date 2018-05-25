function updateStats(last, correct, tries) {
    if(tries > 0) {
        document.getElementById("tries").innerHTML = tries;
        var res = document.getElementById("response");
        if (correct) {
            res.innerText = "Correct Answer";
            res.style.color = "green";
        }
        else {
            res.innerText = "Wrong Answer";
            res.style.color = "red";
        }
        mark = correct ? 4 - tries : 0;
        document.getElementById("mark").innerHTML = `mark: ${mark}`;
    }
    else {
        document.getElementById("tries").innerHTML = 0;
        document.getElementById("filler").checked = true;
        document.getElementById("response").innerText = "";
        document.getElementById("response").style.backgroundColor = null;
        document.getElementById("mark").innerHTML = "";
    }
}

function loadPage() {
    var xhr = new XMLHttpRequest();
    sendDat = { 'uname': window.localStorage.uname, 'token': window.localStorage.token }
    xhr.open('POST', "/getQuestionData", true);
    xhr.send(JSON.stringify(sendDat));

    function processRequest(e) {
        if (xhr.readyState == 4 && xhr.status == 200) {
            var response = JSON.parse(xhr.responseText);
            console.log(response);
            if (response.options) {
                document.getElementById("Question").innerHTML = response.question;
                document.getElementById("o0").innerHTML = response.options[0];
                document.getElementById("o1").innerHTML = response.options[1];
                document.getElementById("o2").innerHTML = response.options[2];
                document.getElementById("o3").innerHTML = response.options[3];
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
    var num =  Array.from(document.getElementsByName("answer")).find(r => r.checked).value;
    console.log(num);
    var xhr2 = new XMLHttpRequest();
    sendDat = { 'uname': window.localStorage.uname, 'token': window.localStorage.token, 'option': num }
    xhr2.open('POST', "/markQuestion", true);
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
    xhr2.open('POST', "/moveQuestion", true);
    xhr2.send(JSON.stringify(sendDat));
    function processRequestSubmit(e) {
        if (xhr2.readyState == 4 && xhr2.status == 200) {
            var response = JSON.parse(xhr2.responseText);
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