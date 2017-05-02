//MAKE GENERIC
function getJSON(urlStr, callback)
{
    return $.ajax({
        url:urlStr,        
        contentType:"application/json; charset=utf-8",
        dataType:"json",
    });
}

$(document).ready(function(){
    //MAKE GENERIC, ALL
    getJSON("http://localhost:3000/home/kmeans/" + 100).done( function(data){
        new Chartkick.ScatterChart("excersisesCompletionAndGradesCluster", data, {xtitle: "Excersise amount", ytitle: "Grade"});    
    });

    getJSON("http://localhost:3000/home/successrate/" + 100).done( function(data){
        new Chartkick.ScatterChart("gradesAndSuccesRate", data, {xtitle: "Correct answers amount", ytitle: "Grade", legend: false, colors: []});    
    });
    var testchart = Chartkick.charts["gradesAndSuccesRate"];
    testchart.updateData({"612":25});

    $("#percentageConfirmation").click(function(){
        var excersiseGradesClusteringChart = Chartkick.charts["excersisesCompletionAndGradesCluster"];
    
        var newUrl = "http://localhost:3000/home/kmeans/" + $("#percentage").val();

        getJSON(newUrl).done( function(data){
            excersiseGradesClusteringChart.updateData(data);
        });
    });
}); 