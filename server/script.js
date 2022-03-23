// Жесткий говнокод, можно сделать гораздо лучше.

var temp = document.getElementById("auqa_temp");
let relay_lighting = document.getElementById("relay_lighting");
let relay_lighting_purple = document.getElementById("relay_lighting_purple");
let relay_air = document.getElementById("relay_air");
let relay_water_out = document.getElementById("relay_water_out");
let relay_water_in = document.getElementById("relay_water_in");
let relay_temp = document.getElementById("relay_temp");

//Cвет
function relay_state_lighting() {
	var request = new XMLHttpRequest();
	request.open("GET", "/relay_status_lighting", true);
	request.onload = function () {
		if (request.readyState == 4 && request.status == 200) {
			var response = request.responseText;
			relay_status = Number.parseInt(response);
			if (relay_status == 0) {
				relay_lighting.classList.add("relay_off");
			} else {
				relay_lighting.classList.add("relay_on");
			}
		}
	};
	request.send();
}

//Cвет
function relay_inverse_lighting() {
	var request = new XMLHttpRequest();
	request.open("GET", "/relay_switch_lighting", false);
	request.send();
	if (request.readyState == 4 && request.status == 200) {
		var response = request.responseText;
		if (response == "0") {
			relay_lighting.classList.remove("relay_on");
			relay_lighting.classList.add("relay_off");
		} else {
			relay_lighting.classList.remove("relay_off");
			relay_lighting.classList.add("relay_on");
		}
	}
}
// Свет ультрафи
function relay_state_lighting_purple() {
	var request = new XMLHttpRequest();
	request.open("GET", "/relay_status_lighting_purple", true);
	request.onload = function () {
		if (request.readyState == 4 && request.status == 200) {
			var response = request.responseText;
			relay_status = Number.parseInt(response);
			if (relay_status == 0) {
				relay_lighting_purple.classList.add("relay_off");
			} else {
				relay_lighting_purple.classList.add("relay_on");
			}
		}
	};
	request.send();
}
// Свет ультрафи
function relay_inverse_lighting_purple() {
	var request = new XMLHttpRequest();
	request.open("GET", "/relay_switch_lighting_purple", false);
	request.send();
	if (request.readyState == 4 && request.status == 200) {
		var response = request.responseText;
		if (response == "0") {
			relay_lighting_purple.classList.remove("relay_on");
			relay_lighting_purple.classList.add("relay_off");
		} else {
			relay_lighting_purple.classList.remove("relay_off");
			relay_lighting_purple.classList.add("relay_on");
		}
	}
}
// Воздух
function relay_state_air() {
	var request = new XMLHttpRequest();
	request.open("GET", "/relay_status_air", true);
	request.onload = function () {
		if (request.readyState == 4 && request.status == 200) {
			var response = request.responseText;
			relay_status = Number.parseInt(response);
			if (relay_status == 0) {
				relay_air.classList.add("relay_off");
			} else {
				relay_air.classList.add("relay_on");
			}
		}
	};
	request.send();
}
// Воздух
function relay_inverse_air() {
	var request = new XMLHttpRequest();
	request.open("GET", "/relay_switch_air", false);
	request.send();
	if (request.readyState == 4 && request.status == 200) {
		var response = request.responseText;
		if (response == "0") {
			relay_air.classList.remove("relay_on");
			relay_air.classList.add("relay_off");
		} else {
			relay_air.classList.remove("relay_off");
			relay_air.classList.add("relay_on");
		}
	}
}
//Вода из аквариума
function relay_state_water_out() {
	var request = new XMLHttpRequest();
	request.open("GET", "/relay_status_water_out", true);
	request.onload = function () {
		if (request.readyState == 4 && request.status == 200) {
			var response = request.responseText;
			relay_status = Number.parseInt(response);
			if (relay_status == 0) {
				relay_water_out.classList.add("relay_off");
			} else {
				relay_water_out.classList.add("relay_on");
			}
		}
	};
	request.send();
}
//Вода из аквариума
function relay_inverse_water_out() {
	var request = new XMLHttpRequest();
	request.open("GET", "/relay_switch_water_out", false);
	request.send();
	if (request.readyState == 4 && request.status == 200) {
		var response = request.responseText;
		if (response == "0") {
			relay_water_out.classList.remove("relay_on");
			relay_water_out.classList.add("relay_off");
		} else {
			relay_water_out.classList.remove("relay_off");
			relay_water_out.classList.add("relay_on");
		}
	}
}

//Вода в аквариум
function relay_state_water_in() {
	var request = new XMLHttpRequest();
	request.open("GET", "/relay_status_water_in", true);
	request.onload = function () {
		if (request.readyState == 4 && request.status == 200) {
			var response = request.responseText;
			relay_status = Number.parseInt(response);
			if (relay_status == 0) {
				relay_water_in.classList.add("relay_off");
			} else {
				relay_water_in.classList.add("relay_on");
			}
		}
	};
	request.send();
}
//Вода в аквариум
function relay_inverse_water_in() {
	var request = new XMLHttpRequest();
	request.open("GET", "/relay_switch_water_in", false);
	request.send();
	if (request.readyState == 4 && request.status == 200) {
		var response = request.responseText;
		if (response == "0") {
			relay_water_in.classList.remove("relay_on");
			relay_water_in.classList.add("relay_off");
		} else {
			relay_water_in.classList.remove("relay_off");
			relay_water_in.classList.add("relay_on");
		}
	}
}

//Обогреватель
function relay_state_temp() {
	var request = new XMLHttpRequest();
	request.open("GET", "/relay_status_temp", true);
	request.onload = function () {
		if (request.readyState == 4 && request.status == 200) {
			var response = request.responseText;
			relay_status = Number.parseInt(response);
			if (relay_status == 0) {
				relay_water_in.classList.add("relay_off");
			} else {
				relay_water_in.classList.add("relay_on");
			}
		}
	};
	request.send();
}
//Обогреватель
function relay_inverse_temp() {
	var request = new XMLHttpRequest();
	request.open("GET", "/relay_switch_temp", false);
	request.send();
	if (request.readyState == 4 && request.status == 200) {
		var response = request.responseText;
		if (response == "0") {
			relay_water_in.classList.remove("relay_on");
			relay_water_in.classList.add("relay_off");
		} else {
			relay_water_in.classList.remove("relay_off");
			relay_water_in.classList.add("relay_on");
		}
	}
}

//получение температуры
function auqa_temp() {
	var request = new XMLHttpRequest();
	request.open("GET", "/auqa_temp", false);
	request.send();
	if (request.status == 200) {
		var response = request.responseText;
		temp.value = response;
	}
}

document.addEventListener("DOMContentLoaded", relay_state_lighting);
document.addEventListener("DOMContentLoaded", relay_state_lighting_purple);
document.addEventListener("DOMContentLoaded", relay_state_air);
document.addEventListener("DOMContentLoaded", relay_state_water_out);
document.addEventListener("DOMContentLoaded", relay_state_water_in);
document.addEventListener("DOMContentLoaded", relay_state_temp);
document.addEventListener("DOMContentLoaded", auqa_temp);

relay_lighting.addEventListener("click", relay_inverse_lighting);
relay_lighting_purple.addEventListener("click", relay_inverse_lighting_purple);
relay_air.addEventListener("click", relay_inverse_air);
relay_water_out.addEventListener("click", relay_inverse_water_out);
relay_water_in.addEventListener("click", relay_inverse_water_in);
relay_temp.addEventListener("click", relay_inverse_temp);
