(function () {
    let count = 1;
    window.setInterval(() => document.getElementsByTagName('h2')[0].innerText = String(count++), 1000)
})()