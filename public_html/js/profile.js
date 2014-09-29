//** Editted by Zach Shroeder

function profileHideAll(){
	$("#statsdiv").hide();
	$("#achievementsdiv").hide();
	$("#inboxdiv").hide();
	$("#searchdiv").hide();
	$("#managediv").hide();
	$("#settingsdiv").hide();
	$("#uploaddiv").hide();
	$("#editdiv").hide();
}
function setProfileButtons(){
	$("#stats").click(function(){
		profileHideAll();
		$("#statsdiv").show();
	});
	$("#achievements").click(function(){
		profileHideAll();
		$("#achievementsdiv").show();
	});
	$("#inbox").click(function(){
		profileHideAll();
		$("#inboxdiv").show();
	});
	$("#search").click(function(){
		profileHideAll();
		$("#searchdiv").show();
	});
	$("#manage").click(function(){
		profileHideAll();
		$("#managediv").show();
	});
	$("#settings").click(function(){
		profileHideAll();
		$("#settingsdiv").show();
	});
	$("#upload").click(function(){
		profileHideAll();
		$("#uploaddiv").show();
	});
	$("#edit").click(function(){
		profileHideAll();
		$("#editdiv").show();
	});
}
