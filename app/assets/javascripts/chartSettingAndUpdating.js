
$(document).ready(function () {
    createChartPercentageChoosers();
    createLinearRegressionRadioBtns();

    handleChartPercentageChoosersClicks();
    handleLinearRegressionLineChoosers();

    drawGradeAvgsChart();
    drawCompletedExcersisesAndGradesClustering(100);
    drawGradesSuccesrateChart(100);
});



/*
CHOOSER CREATIONS
*/

function createChartPercentageChoosers() {
    var percentageChooserHtml =
        '<p style="margin-top: 100px;">Grade Percentage: </p>' +
        '<input type="number" class="percentageChooserValue" max="100"></input>' +
        '<input type="button" class="percentageChooserBtn"value="Confirm"></input>';

    $('.updatableChart').before(percentageChooserHtml);
}

function createLinearRegressionRadioBtns() {
    var LinearRegressionRadioBtnHtml =
        '<p style="margin-top: 20px;">Add/Remove LinearRegression</p>' +
        '<input type="checkbox" class="LinearRegressionButton"></input>';
    $('.LinearRegressionChart').before(LinearRegressionRadioBtnHtml);
}



/*
REGRESSION AND PERCENTAGE RUNTIME
*/

function handleChartPercentageChoosersClicks() {
    var updateFunctionsPerChart =
        {
            "completedexcersisesandgradesclustering": drawCompletedExcersisesAndGradesClustering,
            "gradesuccessrate": drawGradesSuccesrateChart
        };

    $('.percentageChooserBtn').click(function () {
        var newPercentageValue = $(this).prev().val();
        var associatedChartId = $(this).nextAll('.updatableChart').attr('id');

        updateFunctionsPerChart[associatedChartId](newPercentageValue);
    });
}

function handleLinearRegressionLineChoosers() {
    $('.LinearRegressionButton').click(function () {
        var associatedChartId = $(this).nextAll('.LinearRegressionChart').attr('id');
        var myChart = $('#' + associatedChartId).highcharts();

        if ($(this).is(':checked')) {
            var allValuesArray = getAllSeriesData(myChart);

            $.get("http://localhost:3000/home/linearregression/[" + allValuesArray + "]", function (data) {
                drawLinearRegression(myChart, data);
            });
        }
        else {
            removeLinearRegression(myChart);
        }
    });
}

function drawLinearRegression(chart, data) {
    var seriesObj =
        {
            type: 'line',
            id: 'LinearRegression',
            name: 'Linear Regression',
            color: 'rgba(255, 0, 0, 0.70)',
            lineWidth: 1.75,
            allowPointSelect: false,
            dataLabels: {
                enabled: false
            },
            markers: {
                enabled: false
            },
            data: []
        };

    //generify since this is duplicated in the charts methods
    for (i = 0; i < data.length; i++) {
        var x = parseFloat(data[i].x);
        var y = parseFloat(data[i].y);
        seriesObj.data.push([x, y]);
    }
    chart.addSeries(seriesObj);
}

function removeLinearRegression(chart) {
    chart.get('LinearRegression').remove();
}

function getAllSeriesData(chart) {
    allSeriesPoints = Array();
    for (i = 0; i < chart.series.length; i++) 
    {
        var currentChartData = chart.series[i]['data']; 
        for (j = 0; j < currentChartData.length; j++) 
        {
            var x = parseFloat(currentChartData[j].x);
            var y = parseFloat(currentChartData[j].y);
            allSeriesPoints.push([x,y]);
        }
    }
    [].concat.apply([], allSeriesPoints);
    return allSeriesPoints;
}



/*
CHART DRAWING
*/

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
