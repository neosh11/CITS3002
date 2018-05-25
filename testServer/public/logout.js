function logout(){
    window.localStorage.token = "";
    window.localStorage.uname = "";
    window.location.replace("/login");
}
