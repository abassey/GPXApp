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
				$('#log-table').append("<tr><th scope=\"row\"><a href=\"uploads/"+ json["filename"] + "\">" + json["filename"] + "</a></th><td>" + json["version"] + "</td><td>" + json["creator"] + "</td><td>" + json["numWaypoints"] + "</td><td>" + json["numRoutes"] + "</td><td>" + json["numTracks"] + "</td>");
				$('#gpx-dropdown').append("<option value=\"" + json["filename"] + "\"" + ">" + json["filename"] + "</option>");
				$('#files-for-rte').append("<option>" + json["filename"] + "</option>");
				$('#files-db').append("<option>" + json["filename"] + "</option>");
				$('#points-db').append("<option>" + json["filename"] + "</option>");
				$('#points-file-db').append("<option>" + json["filename"] + "</option>");
				alert('Successfully added ' + json["filename"] + ' to View Panel');
			}
        },
        fail: function(error) {
            // Non-200 return, do something with error
			alert('Could not add ' + json["filename"] + ' to View Panel');
            console.log(error); 
        }
    });
    ///////////////
    
    ///////////////displaying routes and tracks
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
				
				$("#rte-table").append("<tr><td> Route " + (i+1) + "</td><td id=\"rte-"+data[i]["numPoints"]+"\">" + data[i]["name"] + "</td><td>" + data[i]["numPoints"] + "</td><td>" + data[i]["len"] + "</td><td>" + data[i]["loop"] + "</td></tr>");
				alert("Successfully displayed Routes in " + filename + " to GPX Panel");
			}
        },
        fail: function(error) {
            // Non-200 return, do something with error
			alert("Could not display Routes in " + filename + " to GPX Panel");
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
				alert("Successfully displayed Tracks in " + filename + " to GPX Panel");
			}
        },
        fail: function(error) {
            // Non-200 return, do something with error
			alert("Could not display Tracks in " + filename + " to GPX Panel");
            console.log(error); 
			}
		});
		
		$('#rename').submit(function(e) {
			e.preventDefault();
			
			console.log("form recieved");
			
			if($('#route-option').prop('checked', true))
			{
				console.log("want route changed");
				
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
					tolerance: tol
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
					tolerance: tol
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
    
    /////////////
    $('#db-login').submit(function(e){
		e.preventDefault();
		
		let username = $("#username").val();
		let password = $("#password").val();
		let dbname = $("#db-name").val();
		
		//console.log(dbname);
		
		$.ajax({
			type: 'get',            //Request type
  				dataType: 'json',       //Data type - we will use JSON for almost everything
  				url: '/dbms/',   //The server endpoint we are connecting to
  				data: {
					username: username,
					password: password,
					dbname: dbname,
				},
  				success: function (data) {
					console.log(data);
					if(data === "Success")
					{
						alert('Successfully Signed In');
						$('#log-button').html("Logout");
						$('#file-route').prop('disabled', false);
						$('#route-point').prop('disabled', false);
						$('#rte-sort-all').prop('disabled', false);
						$('#store-all').prop('disabled', false);
						$('#clear-all').prop('disabled', false);
						$('#db-refresh').prop('disabled', false);
					}else{
						alert('Please Check Your Credentials and Log in Again');
					}
  				},
  				fail: function(error) {
  					// Non-200 return, do something with error
  					console.log(error);
  					alert('Please Check Your Credentials and Log in Again');
  				}
		});
					alert('Successfully Signed In');
					$('#file-route').prop('disabled', false);
					$('#route-point').prop('disabled', false);
					$('#rte-sort-all').prop('disabled', false);
					$('#store-all').prop('disabled', false);
					$('#clear-all').prop('disabled', false);
					$('#db-refresh').prop('disabled', false);
					
		$('#clear-all').click(function(e){
		e.preventDefault();
		
			$.ajax({
				type: 'get',            //Request type
					dataType: 'json',       //Data type - we will use JSON for almost everything
					url: '/delete-all/',   //The server endpoint we are connecting to
					data: {
						username: username,
						password: password,
						dbname: dbname
					},
					success: function (data) {
						console.log(data);
						alert("All Data Successfully Cleared in Database. Refresh DB Status to View Changes.");
					},
					fail: function(error) {
						// Non-200 return, do something with error
						console.log(error);
					}
			});
		});
		
		$('#store-all').click(function(e){
		e.preventDefault();
		
			$.ajax({
				type: 'get',            //Request type
					dataType: 'json',       //Data type - we will use JSON for almost everything
					url: '/storefiles/',   //The server endpoint we are connecting to
					data: {
						username: username,
						password: password,
						dbname: dbname
					},
					success: function (data) {
						console.log(data);
						alert("All Files Successfully Stored in Database. Refresh DB Status to View Changes.");
					},
					fail: function(error) {
						// Non-200 return, do something with error
						console.log(error);
					}
			});
		});
		
		$('#db-refresh').click(function(e){
		e.preventDefault();
		
			$.ajax({
				type: 'get',            //Request type
					dataType: 'json',       //Data type - we will use JSON for almost everything
					url: '/db-status/',   //The server endpoint we are connecting to
					data: {
						username: username,
						password: password,
						dbname: dbname
					},
					success: function (data) {
						console.log(data);
						$('#db-status').html("Database has " + data[0] + " files, " + data[1] + " routes and " + data[2] + " points.");
					},
					fail: function(error) {
						// Non-200 return, do something with error
						console.log(error);
					}
			});
		});
		
		$('#rte-sort-all').click(function(e){
		e.preventDefault();
		
		let choice;
		
	
			
	if($('#sort-by-length').prop("checked") == true) {
			choice = "route_len";
		}else{
			choice = "route_name";
			}
		
		console.log(choice);
		
			$.ajax({
				type: 'get',            //Request type
					dataType: 'json',       //Data type - we will use JSON for almost everything
					url: '/display-rtes/',   //The server endpoint we are connecting to
					data: {
						username: username,
						password: password,
						dbname: dbname,
						choice: choice
					},
					success: function (data) {
						console.log(data);
						$('#sort-all-rtes td').remove();
						for(var i = 0; i < data.length; i++)
						{
							$('#sort-all-rtes').append("<tr><td>" + data[i]["route_name"] + "</td><td>" +data[i]["route_len"] + "</td>");
						}
					},
					fail: function(error) {
						// Non-200 return, do something with error
						console.log(error);
					}
			});
		});
		
		$('#file-route').click(function(e){
		e.preventDefault();
		
		let filename = $("#files-db option:selected").text();
		let choice;
		console.log(filename);
				
		if($('#sort-length').prop("checked") == true) {
			choice = "route_len";
		}else{
			choice = "route_name";
			}
		
			$.ajax({
				type: 'get',            //Request type
					dataType: 'json',       //Data type - we will use JSON for almost everything
					url: '/display-rtes-file/',   //The server endpoint we are connecting to
					data: {
						username: username,
						password: password,
						dbname: dbname,
						filename: filename,
						choice: choice
					},
					success: function (data) {
						console.log(data);
						$('#sort-file-rte tr').remove();
						for(var i = 0; i < data.length; i++)
						{
							//check why this removes the th
							$('#sort-file-rte').append("<tr><td>" + data[i]["route_name"] + "</td><td>" + data[i]["route_len"] + "</td>");
						}
					},
					fail: function(error) {
						// Non-200 return, do something with error
						console.log(error);
					}
			});
		});


	$('#route-point').click(function(e){
		e.preventDefault();
		
		let filename = $("#points-db option:selected").text();
		let rtechoice = $("#route-point-name").val();
		console.log(filename);
		
			$.ajax({
				type: 'get',            //Request type
					dataType: 'json',       //Data type - we will use JSON for almost everything
					url: '/display-points-rte/',   //The server endpoint we are connecting to
					data: {
						username: username,
						password: password,
						dbname: dbname,
						filename: filename,
						rtechoice: rtechoice
					},
					success: function (data) {
						console.log(data);
						$('#sort-points-rte tr').remove();
						for(var i = 0; i < data.length; i++)
						{
							//check why this removes the th
							$('#sort-points-rte').append("<tr><td>" + data[i]["point_index"] + "</td><td>" + data[i]["point_name"] + "</td><td>" + data[i]["latitude"] + "</td><td>" + data[i]["longitude"] + "</td>");
						}
					},
					fail: function(error) {
						// Non-200 return, do something with error
						console.log(error);
					}
			});
		});
		
		$('#point-file').click(function(e){
		e.preventDefault();
		
		let filename = $("#points-file-db option:selected").text();
		console.log(filename);
		
			$.ajax({
				type: 'get',            //Request type
					dataType: 'json',       //Data type - we will use JSON for almost everything
					url: '/display-points-file/',   //The server endpoint we are connecting to
					data: {
						username: username,
						password: password,
						dbname: dbname,
						filename: filename
					},
					success: function (data) {
						console.log(data);
						$('#sort-points-file tr').remove();
						for(var i = 0; i < data.length; i++)
						{
							//check why this removes the th
							$('#sort-points-file').append("<tr><td>" + data[i]["point_index"] + "</td><td>" + data[i]["point_name"] + "</td><td>" + data[i]["latitude"] + "</td><td>" + data[i]["longitude"] + "</td>");
						}
					},
					fail: function(error) {
						// Non-200 return, do something with error
						console.log(error);
					}
			});
		});
		
	});
    /////////////
    
    ////////////
    
    /////////////
    
    
    //last one. dont close past this
});
