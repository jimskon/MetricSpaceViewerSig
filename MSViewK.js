
// JavaScript for Enbedded Space Demo
// Jim Skon, Kenyon College, 2017
var stageW = 2200;
var stageH = 1200;
var nodeSize = 1;
var stage = new Kinetic.Stage({
    container: 'Graph',
    width: stageW,
    height: stageH,
    draggable: false
});
var nodes = new Array;
var layer = new Kinetic.Layer();
$(document).ready(function () {

    
    $("#GraphView").hide();
    
    $("#gen-btn").click(genPoints);

    $("#clear").click(clearResults);

    $('#DisplayButton').click( function () {
	if ($(this).attr("state")=="off") {
	    $(this).attr("state", "on");
	    $(this).text("Show Points");
	    $("#GraphView").show();
	    $("#PointsList").hide();
	} else {
	    $(this).attr("state","off");	    
	    $(this).text("Show Graph");
	    $("#GraphView").hide();
	    $("#PointsList").show();
	}
    });
    
    $(document).ajaxStart(function () {
	$("#loading").show();
    }).ajaxStop(function () {
	$("#loading").hide();
    });

});

function clearResults() {
    $('#points').empty();
}

function addNode(xp,yp) {
    var node = new Kinetic.Circle({
	x: xp+10,
	y: yp+10,
	radius: nodeSize,
	fill:'blue',
	stroke: 'blue',
	strokeWidth: 1,
	draggable: false
    });
    layer.add(node);
    console.log("Node:"+xp+","+yp);
}

function processResults(resultXML) {
    var output = "";
    nodes = new Array;

    //console.log(resultXML);
    var count = 0;
    $('n',resultXML).each(function () {
	var sig = $(this).find("s").text();
	var x = parseInt($(this).find("x").text());
	var y = parseInt($(this).find("y").text());
	var links = $(this).find("l").text();
	addNode(x,y);
	output+="["+count+"] x:"+x+" y:"+y+"->"+links+"<br />";
	count++;
	//output+=$(this).find("signature").text()+" x:"+$(this).find("x").text()+" y:"+$(this).find("y").text()+"<br />";
    });
    stage.add(layer);
    stage.draw();
    $('#points').html(output+"<br />Total:"+count);
}

function genPoints(){
    $('#points').empty();

    $.ajax({
	url: '/cgi-bin/skon_genGraph.cgi?levels='+$('#levels').val(),
	dataType: 'text',
	success: processResults,
	error: function(){alert("Error: Something went wrong");}
    });
}
