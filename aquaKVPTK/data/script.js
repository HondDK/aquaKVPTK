// Жесткий говнокод, можно сделать гораздо лучше.

var temp = document.getElementById("auqa_temp");
var time = document.getElementById("status_time");

let relay_lighting = document.getElementById("relay_lighting");
let relay_lighting_purple = document.getElementById("relay_lighting_purple");
let relay_air = document.getElementById("relay_air");
let relay_water_out = document.getElementById("relay_water_out");
let relay_water_in = document.getElementById("relay_water_in");
let relay_temp = document.getElementById("relay_temp");

let timer_time_ligh_h_on = document.getElementById("timer_time_lighting_h_on");
let timer_time_ligh_m_on = document.getElementById("timer_time_lighting_m_on");
let timer_time_ligh_h_off = document.getElementById(
	"timer_time_lighting_h_off"
);
let timer_time_ligh_m_off = document.getElementById(
	"timer_time_lighting_m_off"
);

let timer_time_ligh_purple_m_on = document.getElementById(
	"timer_time_lighting_purple_m_on"
);
let timer_time_ligh_purple_m_off = document.getElementById(
	"timer_time_lighting_purple_m_off"
);

let timer_air_m_on = document.getElementById("timer_time_air_m_on");
let timer_air_m_off = document.getElementById("timer_time_air_m_off");

let timer_water_out_m_on = document.getElementById("timer_time_water_out_m_on");
let timer_water_out_m_off = document.getElementById(
	"timer_time_water_out_m_off"
);

let timer_food_h_1 = document.getElementById("timer_time_food_h_1");
let timer_food_m_1 = document.getElementById("timer_time_food_m_1");
let timer_food_h_2 = document.getElementById("timer_time_food_h_2");
let timer_food_m_2 = document.getElementById("timer_time_food_m_2");

let install_temp = document.getElementById("installed_temp");

let relay_status;
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
	location.reload();
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
	location.reload();
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
	location.reload();
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
	location.reload();
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
	location.reload();
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
				relay_temp.classList.add("relay_off");
			} else {
				relay_temp.classList.add("relay_on");
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
			relay_temp.classList.remove("relay_on");
			relay_temp.classList.add("relay_off");
		} else {
			relay_temp.classList.remove("relay_off");
			relay_temp.classList.add("relay_on");
		}
	}
	location.reload();
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

//получение времени
function status_time() {
	var request = new XMLHttpRequest();
	request.open("GET", "/status_time", true);
	request.onload = function () {
		if (request.status == 200) {
			time.value = request.responseText;
		} else time.value = "0";
	};
	request.send();
}

//получение времени
function timer_time() {
	var request = new XMLHttpRequest();
	var request2 = new XMLHttpRequest();
	var request3 = new XMLHttpRequest();
	var request4 = new XMLHttpRequest();
	var request6 = new XMLHttpRequest();
	var request8 = new XMLHttpRequest();
	var request10 = new XMLHttpRequest();
	var request11 = new XMLHttpRequest();
	var request12 = new XMLHttpRequest();
	var request13 = new XMLHttpRequest();
	var request14 = new XMLHttpRequest();

	//Освещение
	request.open("GET", "/timer_time_lighting_h_on", true);
	request2.open("GET", "/timer_time_lighting_m_on", true);
	request3.open("GET", "/timer_time_lighting_h_off", true);
	request4.open("GET", "/timer_time_lighting_m_off", true);
	//Ультрафиолет
	request6.open("GET", "/timer_time_lighting_purple_m_off", true);
	//Воздух растениям
	request8.open("GET", "/timer_air_m_off", true);
	//Вода из аквариума к растениям
	request10.open("GET", "/timer_time_water_out_m_off", true);
	//Кормушка
	request11.open("GET", "/timer_time_food_h_1", true);
	request12.open("GET", "/timer_time_food_m_1", true);
	request13.open("GET", "/timer_time_food_h_2", true);
	request14.open("GET", "/timer_time_food_m_2", true);

	request.onload = function () {
		//Освещение
		timer_time_ligh_h_on.value = request.responseText;
		timer_time_ligh_m_on.value = request2.responseText;
		timer_time_ligh_h_off.value = request3.responseText;
		timer_time_ligh_m_off.value = request4.responseText;
		//Ультрафиолет
		timer_time_ligh_purple_m_off.value = request6.responseText;
		//Воздух
		timer_air_m_off.value = request8.responseText;
		//Вода из аквариума к растениям и вода от растений в аквариум
		timer_water_out_m_off.value = request10.responseText;
		//Кормушка
		timer_food_h_1.value = request11.responseText;
		timer_food_m_1.value = request12.responseText;
		timer_food_h_2.value = request13.responseText;
		timer_food_m_2.value = request14.responseText;
	};
	request.send();
	request2.send();
	request3.send();
	request4.send();
	request6.send();
	request8.send();
	request10.send();
	request11.send();
	request12.send();
	request13.send();
	request14.send();
}

document.addEventListener("DOMContentLoaded", relay_state_lighting);
document.addEventListener("DOMContentLoaded", relay_state_lighting_purple);
document.addEventListener("DOMContentLoaded", relay_state_air);
document.addEventListener("DOMContentLoaded", relay_state_water_out);
document.addEventListener("DOMContentLoaded", relay_state_water_in);
document.addEventListener("DOMContentLoaded", relay_state_temp);
document.addEventListener("DOMContentLoaded", auqa_temp);
document.addEventListener("DOMContentLoaded", status_time);
document.addEventListener("DOMContentLoaded", timer_time);

relay_lighting.addEventListener("click", relay_inverse_lighting);
relay_lighting_purple.addEventListener("click", relay_inverse_lighting_purple);
relay_air.addEventListener("click", relay_inverse_air);
relay_water_out.addEventListener("click", relay_inverse_water_out);
relay_water_in.addEventListener("click", relay_inverse_water_in);
relay_temp.addEventListener("click", relay_inverse_temp);
