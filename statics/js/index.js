(function () {
    let count = 1;
    window.setInterval(() => document.getElementsByTagName('h2')[0].innerText = `JS脚本已运行${count++} s`, 1000)
})()