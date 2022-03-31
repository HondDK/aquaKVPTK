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

let tagBody = document.getElementsByTagName('body');


let setup_timer_time_lighting_purple_m_off = document.getElementById("set_timer_time_lighting_purple_m_off");
let btn_set_timer_time_lighting_purple_m_off = document.getElementById("btn_set_timer_time_lighting_purple_m_off");

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

function setup_timer_time_min(){
	var request = new XMLHttpRequest();
	switch(setup_timer_time_lighting_purple_m_off.value){
		case 1:
			request.open("GET", "/setup_timer_time_min_1", false);
		break;
		case 2:
			request.open("GET", "/setup_timer_time_min_2", false);
		break;
		case 3:
			request.open("GET", "/setup_timer_time_min_3", false);
		break;
		case 4:
			request.open("GET", "/setup_timer_time_min_4", false);
		break;
		case 5:
			request.open("GET", "/setup_timer_time_min_5", false);
		break;
		case 6:
			request.open("GET", "/setup_timer_time_min_6", false);
		break;
		case 7:
			request.open("GET", "/setup_timer_time_min_7", false);
		break;
		case 8:
			request.open("GET", "/setup_timer_time_min_8", false);
		break;
		case 9:
			request.open("GET", "/setup_timer_time_min_9", false);
		break;
		case 10:
			request.open("GET", "/setup_timer_time_min_10", false);
		break;
		case 11:
			request.open("GET", "/setup_timer_time_min_11", false);
		break;
		case 12:
			request.open("GET", "/setup_timer_time_min_12", false);
		break;
		case 13:
			request.open("GET", "/setup_timer_time_min_13", false);
		break;
		case 14:
			request.open("GET", "/setup_timer_time_min_14", false);
		break;
		case 15:
			request.open("GET", "/setup_timer_time_min_15", false);
		break;
		case 16:
			request.open("GET", "/setup_timer_time_min_16", false);
		break;
		case 17:
			request.open("GET", "/setup_timer_time_min_17", false);
		break;
		case 18:
			request.open("GET", "/setup_timer_time_min_18", false);
		break;
		case 19:
			request.open("GET", "/setup_timer_time_min_19", false);
		break;
		case 20:
			request.open("GET", "/setup_timer_time_min_20", false);
		break;
		case 21:
			request.open("GET", "/setup_timer_time_min_21", false);
		break;
		case 22:
			request.open("GET", "/setup_timer_time_min_22", false);
		break;
		case 23:
			request.open("GET", "/setup_timer_time_min_23", false);
		break;
		case 24:
			request.open("GET", "/setup_timer_time_min_24", false);
		break;
		case 25:
			request.open("GET", "/setup_timer_time_min_25", false);
		break;
		case 26:
			request.open("GET", "/setup_timer_time_min_26", false);
		break;
		case 27:
			request.open("GET", "/setup_timer_time_min_27", false);
		break;
		case 28:
			request.open("GET", "/setup_timer_time_min_28", false);
		break;
		case 29:
			request.open("GET", "/setup_timer_time_min_29", false);
		break;
		case 30:
			request.open("GET", "/setup_timer_time_min_30", false);
		break;
		case 31:
			request.open("GET", "/setup_timer_time_min_31", false);
		break;
		case 32:
			request.open("GET", "/setup_timer_time_min_32", false);
		break;
		case 33:
			request.open("GET", "/setup_timer_time_min_33", false);
		break;
		case 34:
			request.open("GET", "/setup_timer_time_min_34", false);
		break;
		case 35:
			request.open("GET", "/setup_timer_time_min_35", false);
		break;
		case 36:
			request.open("GET", "/setup_timer_time_min_36", false);
		break;
		case 37:
			request.open("GET", "/setup_timer_time_min_37", false);
		break;
		case 38:
			request.open("GET", "/setup_timer_time_min_38", false);
		break;
		case 39:
			request.open("GET", "/setup_timer_time_min_39", false);
		break;
		case 40:
			request.open("GET", "/setup_timer_time_min_40", false);
		break;
		case 41:
			request.open("GET", "/setup_timer_time_min_41", false);
		break;
		case 42:
			request.open("GET", "/setup_timer_time_min_42", false);
		break;
		case 43:
			request.open("GET", "/setup_timer_time_min_43", false);
		break;
		case 44:
			request.open("GET", "/setup_timer_time_min_44", false);
		break;
		case 45:
			request.open("GET", "/setup_timer_time_min_45", false);
		break;
		case 46:
			request.open("GET", "/setup_timer_time_min_46", false);
		break;
		case 47:
			request.open("GET", "/setup_timer_time_min_47", false);
		break;
		case 48:
			request.open("GET", "/setup_timer_time_min_48", false);
		break;
		case 49:
			request.open("GET", "/setup_timer_time_min_49", false);
		break;
		case 50:
			request.open("GET", "/setup_timer_time_min_50", false);
		break;
		case 51:
			request.open("GET", "/setup_timer_time_min_51", false);
		break;
		case 52:
			request.open("GET", "/setup_timer_time_min_52", false);
		break;
		case 53:
			request.open("GET", "/setup_timer_time_min_53", false);
		break;
		case 54:
			request.open("GET", "/setup_timer_time_min_54", false);
		break;
		case 55:
			request.open("GET", "/setup_timer_time_min_55", false);
		break;
		case 56:
			request.open("GET", "/setup_timer_time_min_56", false);
		break;
		case 57:
			request.open("GET", "/setup_timer_time_min_57", false);
		break;
		case 58:
			request.open("GET", "/setup_timer_time_min_58", false);
		break;
		case 59:
			request.open("GET", "/setup_timer_time_min_59", false);
		break;
		case 60:
			request.open("GET", "/setup_timer_time_min_60", false);
		break;


	}
	request.send();
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
btn_set_timer_time_lighting_purple_m_off.addEventListener("click", setup_timer_time);
