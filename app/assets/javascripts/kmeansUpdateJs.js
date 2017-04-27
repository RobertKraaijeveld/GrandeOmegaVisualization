//MAKE GENERIC
function getExcersisesCompletionAndGradesClusterJSON(newPercentage, callback)
{
    return $.ajax({
        url:"http://localhost:3000/home/kmeans/" + newPercentage,
        contentType:"application/json; charset=utf-8",
        dataType:"json",
    });
}

$(document).ready(function(){
    //MAKE GENERIC, ALL
    getExcersisesCompletionAndGradesClusterJSON(100).done( function(data){
        new Chartkick.ScatterChart("excersisesCompletionAndGradesCluster", data, {xtitle: "Excersise amount", ytitle: "Grade"});    
    });

    $("#percentageConfirmation").click(function(){
        var excersiseGradesClusteringChart = Chartkick.charts["excersisesCompletionAndGradesCluster"];
        
        getExcersisesCompletionAndGradesClusterJSON($("#percentage").val()).done( function(data){
            excersiseGradesClusteringChart.updateData(data);
        });
    });
}); 