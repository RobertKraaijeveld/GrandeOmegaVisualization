
$(document).ready(function () {
    drawGradeAvgsChart();
    drawCompletedExcersisesAndGradesClustering(100);
    drawGradesSuccesrateChart(100);
});


function getChartOptions(optionsObject) {
    var options = {
        chart: {
            type: optionsObject.type
        },
        title: {
            text: optionsObject.title
        },
        xAxis: {
            categories: optionsObject.xLabels,
            title: {
                text: optionsObject.xText
            }
        },
        yAxis: {
            categories: optionsObject.yLabels,
            title: {
                text: optionsObject.yText
            }
        },
        plotOptions: {
            line: {
                dataLabels: {
                    enabled: true
                },
            }
        },
        legend: {
            layout: 'vertical',
            align: 'right',
            verticalAlign: 'middle'
        },
        series: []
    };
    return options;
}

function drawGradeAvgsChart() {
    $(function () {
        $.getJSON('http://localhost:3000/home/gradeavgs', function (data) {
            var optionObj = {
                title: "Average grade per class",
                type: 'column',
                xText: 'Class no.',
                yText: 'Average grade',                
                xLabels: []
            };

            var firstSeriesObj = {name: "Average grade", data: []}
            for (i = 0; i < data.length; i++) {
                var parsedValFloat = parseFloat(data[i].y);

                optionObj.xLabels.push("Class no. " + data[i].x);
                firstSeriesObj.data.push(parsedValFloat);
            }
            var chartOptions = getChartOptions(optionObj);
            chartOptions.series.push(firstSeriesObj);

            $('#gradeaverages').highcharts(chartOptions);
        });
    });
}

function drawCompletedExcersisesAndGradesClustering(studentsGradePercentile) {
    $(function () {
        $.get('http://localhost:3000/home/kmeans/' + studentsGradePercentile, function (data) {
            var options = {
                title: "Amount of correct excersise answers per student vs. grades per student",
                type: 'scatter',
                xText: 'Amount of correct excersise answers',
                yText: 'Grade',                                
            };
            var optionObj = getChartOptions(options);

            for (i = 0; i < data.length; i++) {
                var currSeriesObj = {name: "Cluster " + (i+1), data: []};

                var currClusterDataArray = data[i]["data"];
                for (j = 0; j < currClusterDataArray.length; j++){
                    //add data to current series
                    var x = parseFloat(currClusterDataArray[j].x);
                    var y = parseFloat(currClusterDataArray[j].y);

                    console.log("Cluster " + (i+1) + " x = " + x + " y = " + y);

                    currSeriesObj.data.push([x, y]);
                }
                optionObj.series.push(currSeriesObj);
            }
            $('#completedexcersisesandgradesclustering').highcharts(optionObj);
        });
    });
}

function drawGradesSuccesrateChart(studentsGradePercentile) {
    $(function () {
        $.getJSON('http://localhost:3000/home/successrate/' + studentsGradePercentile, function (data) {
            var optionObj = {
                title: "Grades and amount of excersise successes per student",
                type: 'scatter',
                xText: 'Completed excersises amount',
                yText: 'Grade'
            };

            /*
            GENERIFY THIS SHIT AND REMOVE THE STANDARD SERIES FROM GETCHARTOPTIONS
            */ 

            var chartOptions = getChartOptions(optionObj);
            chartOptions.series = [];
            chartOptions.yAxis.max = 100;

            var xSeriesObj = {name: "Grades and amount of excersise successes", data: []};
            
            for (i = 0; i < data.length; i++) {
                var parsedKeyFloat = parseFloat(data[i]["data"].x);
                var parsedValFloat = parseFloat(data[i]["data"].y);

                xSeriesObj.data.push([parsedKeyFloat, parsedValFloat]);
            } 
            chartOptions.series.push(xSeriesObj);
            
            $('#gradesuccessrate').highcharts(chartOptions);
        });
    });
}

        //MAKE GENERIC, ALL
        /*
        $("#percentageConfirmation").click(function(){
            var excersiseGradesClusteringChart = Chartkick.charts["excersisesCompletionAndGradesCluster"];
        
            var newUrl = "http://localhost:3000/home/kmeans/" + $("#percentage").val();

            getJSON(newUrl).done( function(data){
                excersiseGradesClusteringChart.updateData(data);
            });
        });
        */
