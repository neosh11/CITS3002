document.getElementById("heading").innerHTML = "Welcome " + window.localStorage.uname;
function getMarks() {
    var xhr = new XMLHttpRequest();
    if (window.localStorage.token && window.localStorage.uname) {
        sendDat = { 'uname': window.localStorage.uname, 'token': window.localStorage.token }
        xhr.open('POST', "/getResults", true);
        xhr.send(JSON.stringify(sendDat));
        function processRequest(e) {
            if (xhr.readyState == 4 && xhr.status == 200) {
                var response = JSON.parse(xhr.responseText);
                var remain = 0;
                var percent = response.total / response.out * 100;
                document.getElementById("outof").innerHTML = `${response.total}/${response.out}`;
                document.getElementById("results").innerHTML = percent.toFixed(2) + '%';

                response.done.forEach(element => {
                    if (!element) remain++;
                });
                document.getElementById("remaining").innerHTML = remain + " questions remaining";

                //Programming
                percent = response.ptotal / response.pout * 100;
                document.getElementById("poutof").innerHTML = `${response.ptotal}/${response.pout}`;
                document.getElementById("presults").innerHTML = percent.toFixed(2) + '%';
                remain = 0;
                response.pdone.forEach(element => {
                    if (!element) remain++;
                });
                document.getElementById("premaining").innerHTML = remain + " questions remaining";
            }
        }
    }
    else {
        //error
    }
    xhr.addEventListener("readystatechange", processRequest, false);
}
getMarks();