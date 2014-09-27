//** Editted by Donald Franklin

//********** GLOBAL VARIABLES **********//
var cgiUrl = "cgi-bin/";
var sesKey = "";
var UserID = 0,
	UserName = "Anonymous",
	UserLevel = 0,
	MuteLevel = 0,
	BanLevel = 0;
var uLevelNames = ['NonUser', 'Player', 'Poster', 'Moderator', 'Admin'];
var DLM = "_|"; //** Delimiting value
var timeObj = new Date();
var lastUpdate = Math.floor(timeObj.getTime()/1000)-10,
	chatInstances = 0;
var updateWait = 5000;
var pageTarget = "",
	nextURL = "";
	
//********** SERVER COMMANDS **********//
//** These are commands the client sends and the server responds with
var MAIN_GAMELIST = "GAMES",				//Get list of games +(optional: sort by)
	MAIN_SESSIONLIST = "SESSIONS",			//Get list of sessions +(game id)
	MAIN_POSTGAME = "POST", 				//Start game session +(game id, session name?, password, number of players)
	MAIN_GAMEUPDATE = "UPDATE",				//Get status of game session +(game session id)
	MAIN_CHANGEUSER = "CHANGEUSER",			//Poster sets position +(new user position)
	MAIN_STARTGAME = "START",				//Sent when everything is ready so the server knows to delete the session
	MAIN_JOIN = "JOIN";						//Join game session +(game id)
	
var PROFILE_MYPROFILE = "SHOWME",			//Get info for user's profile
	PROFILE_OTHERPROFILE = "SHOWOTHER",		//Get info for someone elses profile +(profile user id)
	PROFILE_COLOR = "COLOR",				//Set the user's profile color +(string of length 6)
	PROFILE_NEWREG = "NEWREG",				//Randomizes user's registration code
	PROFILE_NEWPASS = "NEWPASS",			//Change user's password +(OldPassword, NewPassword)
	PROFILE_NEWSTATUS = "NEWSTATS";			//Change user's stats +(FirstName, LastName, Email)
	
var ADMIN_USERS = "USERS",					//Get list of users +(optional: name)
	ADMIN_TIMEOUT = "TIMEOUT",				//Set new global timeout +(new value)
	ADMIN_USERLEVEL = "USERLEVEL",			//Set user's level +(user id, new userlevel)
	ADMIN_REGLIST = "REGLIST",				//Get list of registration requests
	ADMIN_BANLIST = "BANLIST",				//Get list of banned or muted users
	ADMIN_BANUSER = "SETBAN",				//Set user ban level +(user id)
	ADMIN_MUTEUSER = "SETMUTE",				//Set user mute level +(user id)
	ADMIN_GAMEDELETE = "GAMEDELETE",		//Remove users game +(game id)
	ADMIN_USERDELETE = "USERDELETE";		//Remove user's account +(user id)
	
var GAMES_MYGAME = "MYGAMES",				//Get list of users games
	GAMES_ADDGAME = "ADDGAME",				//Add a new game +(GameName, Description, URL, MinPlayers, MaxPlayers)
	GAMES_GAMES = "GAMES",					//Get a list of top 15 games. +(sorting: (empty), "RATING", "TIME", "NAME")
	GAMES_REMOVE = "REMOVE";					//Drop game from table +(GameId)

//** Commands that the client receives sometimes instead of the above
var OTHER_TIMEOUT = "TIMEOUT",				//If the user's session timed out this is the only command they get
	OTHER_ERROR = "ERROR";					//This is always the first thing returned if there's an error
	
	

//***************************************************//
//*					UTILITY METHODS					*//
//***************************************************//
function splitData(str){
	var outputArr = [];
	var pos = str.indexOf(DLM),
		esc = "\|",
		startPos = 0,
		tmpStr;
	
	//** Split string and place chunks into an array
	while (pos != -1){
		tmpStr = str.substring(startPos, pos);

		//** Strip escape characters
		tmpStr.replace("\\|", "|");

		//** Add to array
		outputArr.push(tmpStr);
		
		//** Update positions for next iteration
		startPos = pos + 2;
		pos = str.indexOf(DLM, pos+1);
	}
	
	//** Push any remaining characters onto the array
	if (str.slice(-2) == "\n"){
		//** Fix random bug that pops up sometimes
		outputArr.push(str.substring(0, str.length-2).substring(startPos));
	} else {
		outputArr.push(str.substring(startPos));
	}
	
	//** Return array
	return outputArr;
}

function safeStr(str){
	var tmpStr = str.replace("|", "\\|");
	return tmpStr;
}

function elemExists(strIdName){
	//** length is 0 if element doesn't exist
	if ($(strIdName).length > 0){
		return true;
	} else {
		return false;
	}
}

//** Shamelessly borrowed from: css-tricks.com/snippets/javascript/get-url-variables/
function getQueryVariable(variable) {
   var query = window.location.search.substring(1);
   var vars = query.split("&");
   for (var i=0;i<vars.length;i++) {
		   var pair = vars[i].split("=");
		   if(pair[0] == variable){return pair[1];}
   }
   return("");
}



//***************************************************//
//*				HEADER EVENT METHODS				*//
//***************************************************//
function onHeaderLoad(){
	if ( elemExists("#gear") ){
		$('#gear').bind('mouseover', openSubMenu); 
		$('#gear').bind('mouseout', closeSubMenu); 
	}
	if (elemExists("#btnMain")){
		$("#btnMain").click(onMainClick);
	}
	if (elemExists("#btnProfile")){
		$("#btnProfile").click(onProfileClick);
	}
	if (elemExists("#btnAdmin")){
		$("#btnAdmin").click(onAdminClick);
	}
	if (elemExists("#btnDeveloper") && UserLevel >= 100){
		$("#btnDeveloper").click(onDeveloperClick);
		$("#btnDeveloper").show();
	}
	if (elemExists("#btnLogout")){
		$("#btnLogout").click(onLogoutClick);
	}
	
	//** Load main page content by default
	if (elemExists("#btnMain")){
		//** No wait fadeOut so it's not seen
		$("#innerBody").fadeOut(0, function(){$("#innerBody").load("main_content.html", "", function(){
			$("#innerBody").fadeIn(400, function(){
				onMainLoad();
			})
		})});
	}
}

function openSubMenu() { 
	$(this).find('.myMenu').css('visibility', 'visible'); 
}

function closeSubMenu() { 
	$(this).find('ul').css('visibility', 'hidden');
}



//***************************************************//
//*				LOGIN EVENT METHODS					*//
//***************************************************//
function onLoginLoad(){
	if ( elemExists("#log1") ){
		$("#log1").click(onLoginClick);
	}
	if ( elemExists("#loginUser") ){
		$("#loginUser").focus();
		$("#loginUser").keyup(function(e) {
			if (e.keyCode == 13) {
				$( "#log1" ).click();
			}
		});
	}
	if ( elemExists("#loginPass") ){
		$("#loginPass").keyup(function(e) {
			if (e.keyCode == 13) {
				$( "#log1" ).click();
			}
		});
	}
	if ( elemExists("#loginError") ){
		$("#loginError").hide();
	}

	$( "#passInput" ).keyup(function(e) {
		if (e.keyCode == 13) {
			$( "#btnSendLogin" ).click();
		}
	});
}
function onLoginClick(){
	var uName = $("#loginUser").val(), 
		uPass = $("#loginPass").val(),
		strSend;
		
	if (uName == ""){
		onLoginError("Invalid username.");
		return;
	}
	if (uPass == ""){
		onLoginError("Invalid password.");
		return;
	}
	
	//** Compress string and send
	strSend = "" + uName + DLM + uPass;
	$.ajax({url: cgiUrl + "login.cgi", data: strSend, type: "POST", error: onLoginError, success: onLoginSuccess});
}
function onLoginSuccess(data){
	var dataArr = splitData(data);

	if (dataArr[0] == "ERROR"){
		onLoginError(dataArr);
		return;
	}
	sesKey = dataArr[1];
	UserID = parseInt(dataArr[2]);
	UserName = dataArr[3];
	UserLevel = parseInt(dataArr[4]);
	MuteLevel = parseInt(dataArr[5]);
	BanLevel = parseInt(dataArr[6]);
	
	//** Hide content, load new page, show content
	$("#contentBody").fadeOut(600, function(){$("#contentBody").load("main.html", "", function(){
		$("#contentBody").fadeIn(400, function(){
			onHeaderLoad();
		})
	})});
}
function onLoginError(err){
	if ( $.type(err) == "array" ){
		$("#loginError").html(err[1]);
	} else if ( $.type(err) == "string" ){
		$("#loginError").html(err);
	} else if ( $.type(err) == "number" ){
		$("#loginError").html("Error Code: " + err);
	}
	$("#loginError").show();
}



//***************************************************//
//*				LOGIN EVENT METHODS					*//
//***************************************************//
function onLogoutClick(){
	$.ajax({url: cgiUrl + "logout.cgi", data: sesKey, type: "POST", error: onLogoutSuccess, success: onLogoutSuccess});
}
function onLogoutSuccess(data){
	sesKey = "";

	//** Hide content, load new page, show content
	document.location.href = "index.html";
}



//***************************************************//
//*				REGISTER EVENT METHODS				*//
//***************************************************//
function onRegisterLoad(){
	if ( elemExists("#reg1") ){
		$("#reg1").click(onRegisterClick);
	}
	if ( elemExists("#rUsername") ){
		$("#rUsername").focus();
	}
	if ( elemExists("#regError") ){
		$("#regError").hide();
	}
	
	//** Set hidden element value to the GET value r
	if (elemExists("#rRegcode")){
		var regVal = getQueryVariable("r");
		if (regVal == ""){
			//** Auto-redirect if value of r is non-existent
			document.location.href = "index.html";
			return;
		}
		$("#rRegcode").val(regVal);
	}
}
function onRegisterClick(){
	var strSend;
	
	if ($("#rUsername").val().length < 5){
		onRegisterError("Username must be at least 6 characters long.");
		return;
	}
	if ($("#rPassword").val().length < 6){
		onRegisterError("Password must be at least 6 characters long.");
		return;
	}
	if ($("#rPassword").val() != $("#rPassword2").val()){
		onRegisterError("Passwords do not match.");
		return;
	}
	
	//** Compress string and send
	strSend = $("#rUsername").val() + DLM + 
			  $("#rPassword").val() + DLM + 
			  $("#rFname").val() + DLM + 
			  $("#rLname").val() + DLM + 
			  $("#rEmail").val() + DLM +
			  $("#rRegcode").val();
	$.ajax({url: cgiUrl + "register.cgi", data: strSend, type: "POST", error: onRegisterError, success: onRegisterSuccess});
}
function onRegisterSuccess(data){
	var dataArr = splitData(data);

	if (dataArr[0] == "ERROR"){
		onRegisterError(dataArr);
		return;
	}
	
	//** Pass through to login success because it does the same thing on success
	onLoginSuccess(data);
}
function onRegisterError(err){
	if ( $.type(err) == "array" ){
		$("#regError").html(err[1]);
	} else if ( $.type(err) == "string" ){
		$("#regError").html(err);
	} else if ( $.type(err) == "number" ){
		$("#regError").html("Error Code: " + err);
	}
	$("#regError").show();
}


//***************************************************//
//*					PAGE METHODS					*//
//***************************************************//
function onMainLoad(){
	if (elemExists("#btnAddChat")){
		$("#btnAddChat").click(onChatButton);
		$("#txtAddChat").keypress(
		function(evt){
			if(evt.which == 13){
				$("#btnAddChat").click();
				return false;
			}
		});
		
	}
	if (elemExists("#topMenu_JoinGame")){
		$("#topMenu_Wait").height($("#topMenu_JoinGame").height());
		$("#topMenu_Wait").hide();
	}
	if (elemExists("#msgBox")){
		if (chatInstances == 0){
			chatInstances++;
			requestUpdate("START");
		}
	}
	
	requestMainContent();
	$.ajax("cgi-bin/games.cgi", {data: sesKey + DLM + GAMES_GAMES, type: "POST", success: function(data){
			data = splitData(data);
			$("#innerBody").fadeIn(400);
		}, error: function(data){
			data = splitData(data);
			$("#innerBody").fadeIn(400);
		}});
}
function requestMainContent(){
	var gList = document.getElementById("gameList");
	var sList = document.getElementById("sessionList");
	
	//** Clear game list table
	while (gList.childNodes.length > 1){
		gList.removeChild(gList.lastChild);
	}
	
	addGameLine("Simple Player Game", "[Description]", "games/SinglePlayerGame/", "SinglePlayerGame.zip", "#gameList");
	addGameLine("Multi Player Game", "[Description]", "games/MultiPlayerGame/", "", "#gameList");
	
	//** Clear session list table
	while (sList.childNodes.length > 1){
		sList.removeChild(sList.lastChild);
	}
	addSessionLine("Game Name", 3, 6, "#sessionList");
}
function onMainClick(){
	$("#innerBody").fadeOut(600, function(){$("#innerBody").load("main_content.html", "", function(){
		onMainLoad();
		$("#innerBody").fadeIn(400);
	})});
}
function onAdminLoad(){
}
function onAdminClick(){
	$("#innerBody").fadeOut(600, function(){$("#innerBody").load("admin.html", "", function(){
		onAdminLoad();
		$("#innerBody").fadeIn(400);
	})});
}
function onProfileClick(){
	$("#innerBody").fadeOut(600, function(){$("#innerBody").load("profile.html", "", function(){
		onProfileLoad();
	})});
}
function onDeveloperClick(){
	$("#innerBody").fadeOut(600, function(){
		$.ajax("cgi-bin/devconsole.cgi", {data: sesKey + DLM + "DISPLAY", type: "POST", success: function(data){
			$(innerBody).html(data);
			$("#innerBody").fadeIn(400);
		}, error: function(data){
			$(innerBody).html(data);
			$("#innerBody").fadeIn(400);
		}})
	});
}
function onProfileLoad(data){
	//** setProfileButtons is located in profile.js
	setProfileButtons();
	
	$("#profileName").text(UserName);
	$("#profileLevel").text("-" + uLevelNames[UserLevel] + "-");
	
	//** Display ban labels
	var banStatus = "";
	if (MuteLevel > 0){
		banStatus += "<div style='color:FF6666;'>&lt;Muted&gt;</div>";
	}
	if (BanLevel > 0){
		banStatus += "<div style='color:FF6666;'>&lt;Banned&gt;</div>";
	}
	if (banStatus == ""){
		banStatus = "</br>";
	}
	$("#profileBanned").html(banStatus);
	
	//** Show profile
	$.ajax("cgi-bin/profile.cgi", {data: sesKey + DLM + "SHOWME", type: "POST", success: function(data){
			data = splitData(data);
			$("#innerBody").fadeIn(400);
		}, error: function(data){
			data = splitData(data);
			$("#innerBody").fadeIn(400);
		}});
}


//***************************************************//
//*			GLOBAL PAGE LOAD METHODS				*//
//***************************************************//
function loadPage(page, target){
	//** Changes the content of the entire page (including header) with the page at <page>
	//** Store variables globally for convenient event access
	nextURL = page;
	pageTarget = target;
	
	//** Fade out and switch content
	$(pageTarget).fadeOut(600, function(){$("#contentBody").load(nextURL, "", function(){
		$(pageTarget).fadeIn(400, function(){
			onLoadPage();
		})
	})});
}
function onLoadPage(){
	if (elemExists("#btnAddChat")){
		$("#btnAddChat").click(onChatButton);
	}
	if (elemExists("#topMenu_JoinGame")){
		$("#topMenu_Wait").height($("#topMenu_JoinGame").height());
		$("#topMenu_Wait").hide();
	}
	if (elemExists("#msgBox")){
		if (chatInstances == 0){
			chatInstances++;
			requestUpdate("START");
		}
	}
}



//***************************************************//
//*					CHAT METHODS					*//
//***************************************************//
function onChatUpdate(data){
	var dataArr = splitData(data),
		tmpName,
		tmpMsg,
		tmpColor;
	
	//** No data received so return nothing
	if (data == "" || dataArr.length < 3){
		return;
	}
	
	//** Add error message to chat
	if (dataArr[0] == "ERROR"){
		var newDiv = createChatLineDiv(dataArr[0], dataArr[1], "#FF6666");
		$("#msgBox").append(newDiv);
		var objDiv = document.getElementById("msgBox");
		objDiv.scrollTop = objDiv.scrollHeight;
		return;
	}
	
	//** Update time
	lastUpdate = parseInt(dataArr[0]);
	
	//** Add all new lines to chat
	for (var i=1;i<dataArr.length;i+=3){
		tmpName = dataArr[i];
		tmpMsg = dataArr[i+1];
		tmpColor = dataArr[i+2];
		
		var newDiv = createChatLineDiv(tmpName, tmpMsg, "#" + tmpColor);
		$("#msgBox").append(newDiv);
	}
	
	//** Force scroll test
	var objDiv = document.getElementById("msgBox");
	objDiv.scrollTop = objDiv.scrollHeight;
}
function onChatButton(){
	var chatTxt = $("#txtAddChat").val(),
		trimTxt = chatTxt.trim();
	
	if (chatTxt == ""){ return; }
	if (chatTxt.length < 3){ return; }

	$("#txtAddChat").val("");
	
	if (trimTxt.length < 3){ return; }
	
	sendChatMessage(chatTxt);
}
function createChatLineDiv(nameStr, textStr, color){
	//** Error handling
	if (nameStr == ""){
		nameStr = "Anonymous";
	}
	if (typeof color2 == 'undefined'){
		color2 = color;
	}
	if (textStr[0] == '>') color = '#789922';
	
	var lineDiv = document.createElement('div');
	lineDiv.className = 'chatLine';
	
	var nameSpan = document.createElement('span');
	nameSpan.style.color = color2;
	$(nameSpan).text(nameStr + ":");
	
	var textSpan = document.createElement('span');
	textSpan.className = 'userText';
	textSpan.style.color = color;
	textSpan.style.marginLeft = "4px";
	$(textSpan).text(textStr);
	
	lineDiv.appendChild(nameSpan);
	lineDiv.appendChild(textSpan);
	return lineDiv;
}
function requestUpdate(msgCommand){
	//** Update only if necessary
	timeObj = new Date();
	if (typeof msgCommand == "undefined"){
		msgCommand = "UPDATE";
	}
	if (timeObj.getTime() - updateWait > lastUpdate){
		jQuery.ajax( cgiUrl + "chatupdate.cgi", {"data": sesKey + DLM + msgCommand, "type": "POST"} ).done(onChatUpdate);
		if (elemExists("#msgBox")){
			setTimeout(requestUpdate, updateWait);
			return;
		}
	} else if (elemExists("#msgBox")){
		setTimeout(requestUpdate, lastUpdate - timeObj.getTime());
		return;
	}
	chatInstances--;
}
function sendChatMessage(msg){
	//** Update only if necessary
	jQuery.ajax( cgiUrl + "chatsend.cgi", {"data": sesKey + DLM + msg, "type": "POST"} ).done(onChatUpdate);
}



//***************************************************//
//*					GAME METHODS					*//
//***************************************************//
function addGameLine(name, desc, gameURL, downloadURL, destObj){
	var newTR = '<tr><td class="td" width="200px"><img width="200" height="125" alt="Smiley face" src="' + gameURL + 'icon.png">';
		newTR += '</td><td class="td" align="center">' + name;
		newTR += '</td><td class="td" width="40%">' + desc + '<br/>';
	if (downloadURL != ''){
		newTR += '<button type="button" onclick="downloadFile(\'' + gameURL + downloadURL + '\')">Download</button>';
	} else {
		newTR += '<button type="button" onclick="waitForGame();"';
		if (UserLevel < 2){ newTR += ' disabled'; }
		newTR += '>Start Game</button>';
	}
	newTR += '</td></tr>';
	if (typeof destObj == "string"){
		$(destObj).append(newTR);
	} else {
		destObj.append(newTR);
	}
}
function addSessionLine(name, players, maxPlayers, destObj){
	var newTR = '<tr><td class="td">' + name + '</td>';
		newTR += '<td class="td" align="center">' + players + '/' + maxPlayers + '</td>';
		newTR += '<td class="td" align="right"><a href="#">Join</a></td></tr>';
	if (typeof destObj == "string"){
		$(destObj).append(newTR);
	} else {
		destObj.append(newTR);
	}
}
function onGameReady(){
	
}
function onGameQuit(){
	$("#topMenu_Wait").fadeOut(300, function(){$("#topMenu_JoinGame").fadeIn(300)});
}
function waitForGame(gameURL){
	$("#topMenu_JoinGame").fadeOut(300, function(){$("#topMenu_Wait").fadeIn(300)});
}
function openGame(gameURL){
	var gameWin = window.open(gameURL, '_blank');
	gameWin.focus();
}



//****** ONLOAD ******//
$(document).ready(function(){
	//** Run every possible load method just in case.
	onLoginLoad();
	onRegisterLoad();
});