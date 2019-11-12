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
	
	//shoes name of selected file
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
<<<<<<< HEAD
				$('#log-table').append("<tr><th scope=\"row\"><a href=\"uploads/"+ json["filename"] + "\">" + json["filename"] + "</a></th><td>" + json["version"] + "</td><td>" + json["creator"] + "</td><td>" + json["numWaypoints"] + "</td><td>" + json["numRoutes"] + "</td><td>" + json["numTracks"] + "</td>");
				$('#gpx-dropdown').append("<option value=\"" + json["filename"] + "\"" + ">" + json["filename"] + "</option>");
				$('#files-for-rte').append("<option>" + json["filename"] + "</option>");
				alert('Successfully added ' + json["filename"] + ' to View Panel');
=======
				$('#log-table').append("<tr><th scope=\"row\"><a href=\"uploads/"+ json["filename"] + "\">" + json["filename"] + "</a></th><td>" + json["version"] + "</td><td>" + json["creator"] + "</td><td>" + json["numWaypoints"] + "</td><td>" + json["numRoutes"] + "</td><td>" + json["numTracks"]);
				$('#gpx-dropdown').append("<option value=\"" + json["filename"] + "\"" + ">" + json["filename"] + "</option>");
				$('#files-for-rte').append("<option>" + json["filename"] + "</option>");
				$('#alert').html("<div class=\"alert alert-success alert-dismissible fade show\" role=\"alert\"> Successfully added " + json["filename"] + " to View Panel. <button type=\"button\" class=\"close\" data-dismiss=\"alert\" aria-label=\"Close\"><span aria-hidden=\"true\">&times;</span></button></div>");
>>>>>>> 46a3d96151354c01616859af4c78e0f1a39313c3
			}
        },
        fail: function(error) {
            // Non-200 return, do something with error
<<<<<<< HEAD
			alert('Could not add ' + json["filename"] + ' to View Panel');
=======
            $('#alert').html("<div class=\"alert alert-danger alert-dismissible fade show\" role=\"alert\"> Could not add " + json["filename"] + " to View Panel. <button type=\"button\" class=\"close\" data-dismiss=\"alert\" aria-label=\"Close\"><span aria-hidden=\"true\">&times;</span></button></div>");
>>>>>>> 46a3d96151354c01616859af4c78e0f1a39313c3
            console.log(error); 
        }
    });
    ///////////////
    
<<<<<<< HEAD
    ///////////////displaying routes and tracks
=======
    ///////////////
>>>>>>> 46a3d96151354c01616859af4c78e0f1a39313c3
    $('#gpx-dropdown').change(function(){
		let filename = $("#gpx-dropdown option:selected").text();
		
		console.log(filename);
		
		$('#rte-table tr').remove();
		$('#trk-table tr').remove();
		
		$.ajax({
        type: 'get',            //Request type
        dataType: 'json',       //Data type - we will use JSON for almost everything 
        url: '/rte/' + filename,   //The server endpoint we are connecting to
        
        success: function (data) {
			console.log(data);
            for(var i = 0; i < data.length; i++)
            {
				console.log(data[i]);
				
<<<<<<< HEAD
				$("#rte-table").append("<tr><td> Route " + (i+1) + "</td><td id=\"rte-"+data[i]["numPoints"]+"\">" + data[i]["name"] + "</td><td>" + data[i]["numPoints"] + "</td><td>" + data[i]["len"] + "</td><td>" + data[i]["loop"] + "</td></tr>");
				alert("Successfully displayed Routes in " + filename + " to GPX Panel");
=======
				$("#rte-table").append("<tr><td> Route " + (i+1) + "</td><td>" + data[i]["name"] + "</td><td>" + data[i]["numPoints"] + "</td><td>" + data[i]["len"] + "</td><td>" + data[i]["loop"] + "</td></tr>");
				$('#alert').html("<div class=\"alert alert-success alert-dismissible fade show\" role=\"alert\"> Successfully displayed Routes in " + filename + " to GPX Panel. <button type=\"button\" class=\"close\" data-dismiss=\"alert\" aria-label=\"Close\"><span aria-hidden=\"true\">&times;</span></button></div>");
>>>>>>> 46a3d96151354c01616859af4c78e0f1a39313c3
			}
        },
        fail: function(error) {
            // Non-200 return, do something with error
<<<<<<< HEAD
			alert("Could not display Routes in " + filename + " to GPX Panel");
=======
            $('#alert').html("<div class=\"alert alert-danger alert-dismissible fade show\" role=\"alert\"> Could not display Routes in " + filename + " to GPX Panel. <button type=\"button\" class=\"close\" data-dismiss=\"alert\" aria-label=\"Close\"><span aria-hidden=\"true\">&times;</span></button></div>");
>>>>>>> 46a3d96151354c01616859af4c78e0f1a39313c3
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
<<<<<<< HEAD
				alert("Successfully displayed Tracks in " + filename + " to GPX Panel");
=======
				$('#alert').html("<div class=\"alert alert-success alert-dismissible fade show\" role=\"alert\"> Successfully displayed Tracks in " + filename + " to GPX Panel. <button type=\"button\" class=\"close\" data-dismiss=\"alert\" aria-label=\"Close\"><span aria-hidden=\"true\">&times;</span></button></div>");
>>>>>>> 46a3d96151354c01616859af4c78e0f1a39313c3
			}
        },
        fail: function(error) {
            // Non-200 return, do something with error
<<<<<<< HEAD
			alert("Could not display Tracks in " + filename + " to GPX Panel");
=======
            $('#alert').html("<div class=\"alert alert-danger alert-dismissible fade show\" role=\"alert\"> Could not display Tracks in " + filename + " to GPX Panel. <button type=\"button\" class=\"close\" data-dismiss=\"alert\" aria-label=\"Close\"><span aria-hidden=\"true\">&times;</span></button></div>");
>>>>>>> 46a3d96151354c01616859af4c78e0f1a39313c3
            console.log(error); 
			}
		});
		
		$('#rename').submit(function(e) {
			e.preventDefault();
			
			console.log("form recieved");
			
			if($('#route-option').prop('checked', true))
			{
				console.log("want route changed");
				
<<<<<<< HEAD
				let index = parseInt($("#component").val());
				let newname = $("#new-name").val();
				console.log(filename);
				console.log(index);
				
				alert("To update Route " + index + "'s name to " + newname + " in " + filename);
				
				$.ajax({
  				type: 'get',            //Request type
  				dataType: 'json',       //Data type - we will use JSON for almost everything
  				url: '/rename-rte/',   //The server endpoint we are connecting to
  				data: {
					filename: filename,
					newname: newname,
					index: index
				},
  				success: function (data) {
				
				alert("Successfully renamed Route in " + filename);

					},
					fail: function(error) {
						// Non-200 return, do something with error
				alert("Could not rename Route in" + filename);

						console.log(error);
					}
				});
			}
			
			if($('#track-option').prop('checked', true))
			{
				console.log("want track changed");
				
				let index = parseInt($("#component").val());
				let newname = $("#new-name").val();
				console.log(filename);
				console.log(index);
				
				alert("To update Track " + index + "'s name to " + newname + " in " + filename);
				
				$.ajax({
  				type: 'get',            //Request type
  				dataType: 'json',       //Data type - we will use JSON for almost everything
  				url: '/rename-trk/',   //The server endpoint we are connecting to
  				data: {
					filename: filename,
					newname: newname,
					index: index
				},
  				success: function (data) {
					alert("Successfully renamed Track in " + filename);
					},
					fail: function(error) {
						// Non-200 return, do something with error
					alert("Could not rename Track in " + filename);

						console.log(error);
					}
				});
			}
			
		});
	
	});
    
    //////////////
    
    //////////////making new gpx file
    $('#new-gpx').submit(function(e){
		e.preventDefault();
		
		let filename = $("#fname").val() + ".gpx";
		console.log(filename);
		
		let version = $("#version").val();
		let creator = $("#creator").val();
		
		let gpx = {
			'version' : parseFloat(version),
			'creator' : creator
		};
		
		let gpxJSON = JSON.stringify(gpx);
		console.log(gpxJSON); //it adds quotation marks around the version nmuber. need to remove
		
		$.ajax({
  				type: 'get',            //Request type
  				dataType: 'json',       //Data type - we will use JSON for almost everything
  				url: '/gpxcreate/',   //The server endpoint we are connecting to
  				data: {
					filename: filename,
					gpxJSON: gpxJSON
				},
  				success: function (data) {

				$('#log-table').append("<tr><th scope=\"row\"><a href=\"uploads/"+ filename + "\">" + filename + "</a></th><td>" + data["version"] + "</td><td>" + data["creator"] + "</td><td>" + data["numWaypoints"] + "</td><td>" + data["numRoutes"] + "</td><td>" + data["numTracks"] + "</td>");
				$('#gpx-dropdown').append("<option value=\"" + filename + "\"" + ">" + filename + "</option>");
				$('#files-for-rte').append("<option>" + filename + "</option>");
				alert("Successfully created " + filename + " and now in File Log Panel");

  					console.log(data);
  				},
  				fail: function(error) {
  					// Non-200 return, do something with error
                alert("Could not create " + filename);

  					console.log(error);
  				}
  			});
  			
  			//$('#log-table').append("<tr><th scope=\"row\"><a href=\"uploads/"+ gpxJS["filename"] + "\">" + gpxObj["filename"] + "</a></th><td>" + gpxObj["version"] + "</td><td>" + gpxObj["creator"] + "</td><td>0</td><td>0</td><td>0</td>");
			//$("#alert").append("<div class=\"alert alert-success alert-dissmissble\" role=\"alert\">" + "Successfully created " + filename + "</div>");
	});
    
    //////////////
    
    //////////////finding the path between
    $('#path-between').submit(function (e){
		e.preventDefault();
		
		let startlat = $('#start-lat').val();
		let startlong = $('#start-long').val();
		let endlat = $('#end-lat').val();
		let endlong = $('#end-long').val();
		let tol = $('#tolerance').val();
		
		let	startLat = parseFloat(startlat);
		let	startLong = parseFloat(startlong);
		let	endLat = parseFloat(endlat);
		let	endLong = parseFloat(endlong);
		let	tolerance = parseFloat(tol);
		
		$("#rte-bt-table").append("<tr><td> Route " + (0) + "</td><td>Dummy Route</td><td>7</td><td>500</td><td>true</td></tr>");
		$("#trk-bt-table").append("<tr><td> Track " + (0) + "</td><td>Dummy Track</td><td>450</td><td>false</td></tr>");
		//let pathJSON = JSON.stringify(path);
		//console.log(pathJSON);
		
		$.ajax({
  				type: 'get',            //Request type
  				dataType: 'json',       //Data type - we will use JSON for almost everything
  				url: '/inbetween-rte/',   //The server endpoint we are connecting to
  				data: {
					startLat: startLat,
					startLong: startLong,
					endLat: endLat,
					endLong: endLong,
					tolerance: tol,
				},
  				success: function (data) {
				
					for(var i = 0; i < data.length; i++)
					{
						let json = JSON.parse(data[i]);
						
						for(var j = 0; j < json.length; j++)
						{
							
							//console.log(json[i]);
							$("#rte-bt-table").append("<tr><td> Route " + (j+1) + "</td><td>" + json[j]["name"] + "</td><td>" + json[j]["numPoints"] + "</td><td>" + json[j]["len"] + "</td><td>" + json[j]["loop"] + "</td></tr>");
							alert("Successfully displayed All Routes Between Desired Path");
						}
					}

  					console.log(data);
  				},
  				fail: function(error) {
  					// Non-200 return, do something with error
					alert("Could not display All Routes Between Desired Path");
  					console.log(error);
  				}
  			});
  			
  			$.ajax({
  				type: 'get',            //Request type
  				dataType: 'json',       //Data type - we will use JSON for almost everything
  				url: '/inbetween-trk/',   //The server endpoint we are connecting to
  				data: {
					startLat: startLat,
					startLong: startLong,
					endLat: endLat,
					endLong: endLong,
					tolerance: tol,
				},
  				success: function (data) {
				
					for(var i = 0; i < data.length; i++)
					{
						let json = JSON.parse(data[i]);
						
						for(var j = 0; j < json.length; j++)
						{
							
							$("#trk-bt-table").append("<tr><td> Track " + (j+1) + "</td><td>" + json[j]["name"] + "</td><td>" + json[j]["len"] + "</td><td>" + json[j]["loop"] + "</td></tr>");
							alert("Successfully displayed All Tracks Between Desired Path");
						}
					}

  					console.log(data);
  				},
  				fail: function(error) {
  					// Non-200 return, do something with error
					alert("Could not display All Tracks Between Desired Path");
  					console.log(error);
  				}
  			});
	});
    //////////////
    
    //////////////add a route
    $('#add-rte').submit(function(e) {
		e.preventDefault();
		
		let filename = $("#files-for-rte option:selected").text();
		console.log(filename);
		
		let rName = $("#rte-name").val();
		console.log(rName);
		
		
		var latitudes = $(".rtept-lat").map(function(){ return this.value}).get();
		var longitudes = $(".rtept-long").map(function(){ return this.value}).get();
		
		var coords = [];
		$.each(latitudes, function(index,value){
			coords.push({"lat": parseFloat(value), "lon": parseFloat(longitudes[index])});
		});
		
		var rem = coords.splice(0, 1);
		
		console.log(coords);
		
		let waypoints = JSON.stringify(coords);
		console.log("waypoints " + waypoints);
		
		let rteName = {
			name: rName
		};
		
		let routeName = JSON.stringify(rteName);
		console.log(routeName);
		
		$.ajax({
  				type: 'get',            //Request type
  				dataType: 'json',       //Data type - we will use JSON for almost everything
  				url: '/add-route/',   //The server endpoint we are connecting to
  				data: {
					filename: filename,
					routeName: routeName,
					waypoints: waypoints,
					length: coords.length
				},
  				success: function (data) {
					console.log(data);
					
					alert("Successfully added Route to " + filename + " Reload to View Changes");
  				},
  				fail: function(error) {
  					// Non-200 return, do something with error
					alert("Could not add Route to " + filename);
  					console.log(error);
  				}
  			});
  			
  			alert("Successfully added Route to " + filename + " Reload to View Changes");
		
	});
    /////////////
    
=======
				let rteObj = new Object();
				rteObj.oldname = $("#old-name").val();
				rteObj.newname = $("#new-name").val();
				
				let rteJSON = JSON.stringify(rteObj);
				console.log(rteJSON);
				
				$.ajax({
  				type: 'post',            //Request type
  				dataType: 'text',       //Data type - we will use JSON for almost everything
  				url: '/rename/' + filename + rteJSON,   //The server endpoint we are connecting to
  				success: function (data) {

				$("#alert").append("<div class=\"alert alert-success\" role=\"alert\">" + "Successfully renamed " + rteObj.oldname + "to " + rteObj.newname + "in " + filename + "</div>");

						console.log(data);
					},
					fail: function(error) {
						// Non-200 return, do something with error
				$("#alert").append("<div class=\"alert alert-danger\" role=\"alert\">" + "Could not rename " + rteObj.oldname + "to " + rteObj.newname + "in " + filename + "</div>");

						console.log(error);
					}
				});
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
>>>>>>> 46a3d96151354c01616859af4c78e0f1a39313c3
    
    //last one. dont close past this
});
