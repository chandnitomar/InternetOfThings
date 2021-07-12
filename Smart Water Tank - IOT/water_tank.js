var waterLevel;
var flag=true;

FusionCharts.ready(function(){
            var chartObj = new FusionCharts({
    type: 'cylinder',
    dataFormat: 'json',
    renderAt: 'chart-container',
    width: '300',
    height: '450',
    dataSource: {
        "chart": {
            "theme": "fusion",
            "caption": "Water Level in Tank",
            "subcaption": "",
            "lowerLimit": "0",
            "upperLimit": "100",
            "lowerLimitDisplay": "Empty",
            "upperLimitDisplay": "Full",
            "numberSuffix": "%",
            "showValue": "1",
            "chartBottomMargin": "55",
            "showValue": "0",
			"refreshInterval": "1",
			"refreshInstantly": "1",
			"cylFillColor": "#b3e6ff",
			"cylradius": "100",
			"cylheight": "300"
        },
        "value": "70",
        "annotations": {
            "origw": "400",
            "origh": "290",
            "autoscale": "1",
            "groups": [{
                "id": "range",
                "items": [{
                    "id": "rangeBg",
                    "type": "rectangle",
                    "x": "$canvasCenterX-75",
                    "y": "$chartEndY-40",
                    "tox": "$canvasCenterX +50",
                    "toy": "$chartEndY-80",
                    "fillcolor": "#fff25e"
                }, {
                    "id": "rangeText",
                    "type": "Text",
                    "fontSize": "20",
                    "fillcolor": "#333333",
                    "text": "Loading...",
                    "x": "$chartCenterX-40",
                    "y": "$chartEndY-60"
                }]
            }]
        }

    },
    "events": {
        "rendered": function(evtObj, argObj) {
            evtObj.sender.chartInterval = setInterval(function() {
                getapi();
                if (waterLevel <= 33 && flag)
                {
                    sendEmail();
                    flag=false;
                }
                if(waterLevel > 33){
                    flag=true;
                }
                console.log(waterLevel);
                evtObj.sender.feedData && evtObj.sender.feedData("&value=" + waterLevel);
            }, 1000);
        },
        //Using real time update event to update the annotation
        //showing available volume of Diesel
        "realTimeUpdateComplete": function(evt, arg) {
            var annotations = evt.sender.annotations,
                dataVal = evt.sender.getData(),
                colorVal = (dataVal >= 75) ? "#6caa03" : ((dataVal <= 25) ? "#e44b02" : "#f8bd1b");
            //Updating value
            annotations && annotations.update('rangeText', {
                "text": dataVal + "%"
            });
            //Changing background color as per value
            annotations && annotations.update('rangeBg', {
                "fillcolor": colorVal
            });

        },
        "disposed": function(evt, arg) {
            clearInterval(evt.sender.chartInterval);
        }
    }
}
);
chartObj.render();
});


const api_url =
    "https://api.thingspeak.com/channels/1358838/feeds.json?api_key=2KJLA50SRMEP7T5I&results=1";
//function to read data from thingspeak cloud
async function getapi() {    
    // Storing response
    const response = await fetch(api_url);
    data = await response.json();
    waterLevel = data["feeds"][0]["field1"];
        
}

//function to send email 
function sendEmail() {
    Email.send({
    Host: "smtp.gmail.com",
    Username : "gunjan.vinzuda@gmail.com",
    Password : "gunjan123",
    To : "gvinzuda35@gmail.com",
    From : "gunjan.vinzuda@gmail.com",
    Subject : "Water Level is low",
    Body : "The water level in tank is now " + waterLevel + "%",
    }).then(
        message => alert("mail sent successfully")
        // console.log("meail send")
    );
}