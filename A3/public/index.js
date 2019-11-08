// Put all onload AJAX calls here, and event listeners
$(document).ready(function() {
	
	function clearForms()
    {
      var i;
      for (i = 0; (i < document.forms.length); i++) {
        document.forms[i].reset();
      }
    }
	
	clearForms();
	
	//add more fields to gpx form
	
	$(".custom-file-input").on("change", function() {
				  var fileName = $(this).val().split("\\").pop();
				  $(this).siblings(".custom-file-label").addClass("selected").html(fileName);
				});
				
	//Clone the hidden element and shows it
		$('.add-one').click(function(){
		  $('.dynamic-element').first().clone().appendTo('.dynamic-stuff').show();
		  attach_delete();
		});


		//Attach functionality to delete buttons
		function attach_delete(){
		  $('.delete').off();
		  $('.delete').click(function(){
			console.log("click");
			$(this).closest('.form-group').remove();
		  });
		}
	
	
	//Denis's stuff
	
    // On page-load AJAX Example
  
    //////////////all the stuff to do with files and tables
    $.ajax({
        type: 'get',            //Request type
        dataType: 'json',       //Data type - we will use JSON for almost everything 
        url: '/gpx',   //The server endpoint we are connecting to
        
        success: function (data) {
            for(var i = 0; i < data.length; i++)
            {
				let json = JSON.parse(data[i]);
				$('#log-table').append("<tr><th scope=\"row\"><a href=\"uploads/"+ json["filename"] + "\">" + json["filename"] + "</a></th><td>" + json["version"] + "</td><td>" + json["creator"] + "</td><td>" + json["numWaypoints"] + "</td><td>" + json["numRoutes"] + "</td><td>" + json["numTracks"]);
				$('#gpx-dropdown').append("<option value=\"" + json["filename"] + "\"" + ">" + json["filename"] + "</option>");
				$('#files-for-rte').append("<option>" + json["filename"] + "</option>");
				$('#alert').html("<div class=\"alert alert-success alert-dismissible fade show\" role=\"alert\"> Successfully added " + json["filename"] + " to View Panel. <button type=\"button\" class=\"close\" data-dismiss=\"alert\" aria-label=\"Close\"><span aria-hidden=\"true\">&times;</span></button></div>");
			}
        },
        fail: function(error) {
            // Non-200 return, do something with error
            $('#alert').html("<div class=\"alert alert-danger alert-dismissible fade show\" role=\"alert\"> Could not add " + json["filename"] + " to View Panel. <button type=\"button\" class=\"close\" data-dismiss=\"alert\" aria-label=\"Close\"><span aria-hidden=\"true\">&times;</span></button></div>");
            console.log(error); 
        }
    });
    ///////////////
    
    ///////////////
    $('#gpx-dropdown').change(function(){
		let filename = $("#gpx-dropdown option:selected").text();
		
		console.log(filename);
		
		$.ajax({
        type: 'get',            //Request type
        dataType: 'json',       //Data type - we will use JSON for almost everything 
        url: '/rte/' + filename,   //The server endpoint we are connecting to
        
        success: function (data) {
			console.log(data);
            for(var i = 0; i < data.length; i++)
            {
				console.log(data[i]);
				
				$("#rte-table").append("<tr><td> Route " + (i+1) + "</td><td>" + data[i]["name"] + "</td><td>" + data[i]["numPoints"] + "</td><td>" + data[i]["len"] + "</td><td>" + data[i]["loop"] + "</td></tr>");
				$('#alert').html("<div class=\"alert alert-success alert-dismissible fade show\" role=\"alert\"> Successfully displayed Routes in " + filename + " to GPX Panel. <button type=\"button\" class=\"close\" data-dismiss=\"alert\" aria-label=\"Close\"><span aria-hidden=\"true\">&times;</span></button></div>");
			}
        },
        fail: function(error) {
            // Non-200 return, do something with error
            $('#alert').html("<div class=\"alert alert-danger alert-dismissible fade show\" role=\"alert\"> Could not display Routes in " + filename + " to GPX Panel. <button type=\"button\" class=\"close\" data-dismiss=\"alert\" aria-label=\"Close\"><span aria-hidden=\"true\">&times;</span></button></div>");
            console.log(error); 
			}
		});
		
		$.ajax({
        type: 'get',            //Request type
        dataType: 'json',       //Data type - we will use JSON for almost everything 
        url: '/trk/' + filename,   //The server endpoint we are connecting to
        
        success: function (data) {
			console.log(data);
            for(var i = 0; i < data.length; i++)
            {
				console.log(data[i]);
				
				$("#trk-table").append("<tr><td> Track " + (i+1) + "</td><td>" + data[i]["name"] + "</td><td>" + data[i]["len"] + "</td><td>" + data[i]["loop"] + "</td></tr>");
				$('#alert').html("<div class=\"alert alert-success alert-dismissible fade show\" role=\"alert\"> Successfully displayed Tracks in " + filename + " to GPX Panel. <button type=\"button\" class=\"close\" data-dismiss=\"alert\" aria-label=\"Close\"><span aria-hidden=\"true\">&times;</span></button></div>");
			}
        },
        fail: function(error) {
            // Non-200 return, do something with error
            $('#alert').html("<div class=\"alert alert-danger alert-dismissible fade show\" role=\"alert\"> Could not display Tracks in " + filename + " to GPX Panel. <button type=\"button\" class=\"close\" data-dismiss=\"alert\" aria-label=\"Close\"><span aria-hidden=\"true\">&times;</span></button></div>");
            console.log(error); 
			}
		});
	
	});
    
    //////////////
    
    //////////////making new gpx file
    $('#new-gpx').submit(function(e){
		e.preventDefault();
		
		let filename = $("#fname").val() + ".gpx";
		console.log(filename);
		
		
		let gpxObj = new Object();
		gpxObj.version = $("#version").val();
		gpxObj.creator = $("#creator").val();
		
		let gpxJSON = JSON.stringify(gpxObj);
		console.log(gpxJSON); //it adds quotation marks around the version nmuber. need to remove
		
		$.ajax({
  				type: 'post',            //Request type
  				dataType: 'text',       //Data type - we will use JSON for almost everything
  				url: '/gpxcreate/' + filename + gpxJSON,   //The server endpoint we are connecting to
  				success: function (data) {

            $("#alert").append("<div class=\"alert alert-success\" role=\"alert\">" + "Successfully created " + filename + "</div>");

  					console.log(data);
  				},
  				fail: function(error) {
  					// Non-200 return, do something with error
            $("#alert").append("<div class=\"alert alert-danger\" role=\"alert\">" + "Could not create " + filename + "</div>");

  					console.log(error);
  				}
  			});
		
	});
    
    //////////////
    

    // Event listener form example , we can use this instead explicitly listening for events
    // No redirects if possible
    $('#someform').submit(function(e){
        $('#blah').html("Form has data: "+$('#entryBox').val());
        e.preventDefault();
        //Pass data to the Ajax call, so it gets passed to the server
        $.ajax({
            //Create an object for connecting to another waypoint
        });
    });
    
    //last one. dont close past this
});
