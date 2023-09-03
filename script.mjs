WorkerScript.onMessage = function(message) {
    // ... long-running operations and calculations are done here
    // 重新发送信号
    console.log("message="+message.x)
    var arrmsg = message.x.split("|");
    console.log("ip="+arrmsg[0]+" port="+arrmsg[1]+" user="+arrmsg[2]+" passwd="+arrmsg[3])
    if (arrmsg.length > 3)
    {
//        var retval = operProxy.checkSocks5Tcp(arrmsg[0], arrmsg[1], arrmsg[2], arrmsg[3])
        var retval = operProxy.checkSocks5Tcp(arrmsg[0], arrmsg[1], arrmsg[2], arrmsg[3])
        if (retval === 0)
        {
            console.log("succeed")
        }
        else
        {
            console.log("not succeed")
        }
    }


    WorkerScript.sendMessage({ 'reply': 'Mouse is at ' + message.x + ',' + message.y })
}
