
$(document).ready(function () {
    createChartPercentageChoosers();

    handleChartPercentageChoosersClicks();
    createRegressionLinesChoosers();

    drawGradeAvgsChart();
    drawCompletedExcersisesAndGradesClustering(100);
    drawGradesSuccesrateChart(100);
});


function handleChartPercentageChoosersClicks() {
    //make ids and functionnames so this obj is not necessary anymore
    var updateFunctionsPerChart =
        {
            "completedexcersisesandgradesclustering": drawCompletedExcersisesAndGradesClustering,
            "gradesuccessrate": drawGradesSuccesrateChart
        };

    $('.percentageChooserBtn').click(function () {
        var newPercentageValue = $(this).prev().val();
        var associatedChartId = $(this).next().attr('id');

        updateFunctionsPerChart[associatedChartId](newPercentageValue);
    });
}

function createRegressionLinesChoosers() {
    //if button pressed and no linear regression yet
        //get xvalues series from button.next.chart etc
        //pass to controller
        //add new series with returndata
        //profit
    //else
        //remove series with id 'regression'

    $('.regressionButton').click(function () {
        if ($(this).is(':checked')) 
        {
             alert("it's checked"); 
        }
        else
        {
             alert("it's unchecked");             
        }
    });

}



function createChartPercentageChoosers() {
    var percentageChooserHtml =
        '<p style="margin-top: 100px;">Grade Percentage: </p>' +
        '<input type="number" class="percentageChooserValue" max="100"></input>' +
        '<input type="button" class="percentageChooserBtn"value="Confirm"></input>';

    $('.updatableChart').before(percentageChooserHtml);
}


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

            var firstSeriesObj = { name: "Average grade", data: [] }
            for (i = 0; i < data.length; i++) {
                var parsedValFloat = parseFloat(data[i].y);

                optionObj.xLabels.push("Class no. " + data[i].x);
                firstSeriesObj.data.push(parsedValFloat);
            }

            var chartOptions = getChartOptions(optionObj);
            chartOptions.series.push(firstSeriesObj);

            chartOptions.yAxis.max = 100;
            chartOptions.yAxis.min = 0;


            $('#gradeaverages').highcharts(chartOptions);
        });
    });
}

function drawCompletedExcersisesAndGradesClustering(studentsGradePercentage) {
    $(function () {
        $.get('http://localhost:3000/home/kmeans/' + studentsGradePercentage, function (data) {
            var options = {
                title: "Amount of correct excersise answers per student vs. grades per student",
                type: 'scatter',
                xText: 'Amount of correct excersise answers',
                yText: 'Grade',
            };
            var optionObj = getChartOptions(options);
            optionObj.yAxis.max = 100;
            optionObj.yAxis.min = 0;


            for (i = 0; i < data.length; i++) {
                var currSeriesObj = { name: "Cluster " + (i + 1), data: [] };

                var currClusterDataArray = data[i]["data"];
                for (j = 0; j < currClusterDataArray.length; j++) {
                    //add data to current series
                    var x = parseFloat(currClusterDataArray[j].x);
                    var y = parseFloat(currClusterDataArray[j].y);

                    currSeriesObj.data.push([x, y]);
                }
                optionObj.series.push(currSeriesObj);
            }
            $('#completedexcersisesandgradesclustering').highcharts(optionObj);
        });
    });
}

function drawGradesSuccesrateChart(studentsGradePercentage) {
    $(function () {
        $.getJSON('http://localhost:3000/home/successrate/' + studentsGradePercentage, function (data) {
            var optionObj = {
                title: "Grades and amount of excersise successes per student",
                type: 'scatter',
                xText: 'Completed excersises amount',
                yText: 'Grade'
            };

            var chartOptions = getChartOptions(optionObj);
            chartOptions.series = [];
            chartOptions.yAxis.max = 100;
            chartOptions.yAxis.min = 0;


            var xSeriesObj = { name: "Grades and amount of excersise successes", data: [] };

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
