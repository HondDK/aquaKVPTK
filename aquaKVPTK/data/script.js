// Жесткий говнокод, можно сделать гораздо лучше.

var temp = document.getElementById("auqa_temp");
var time = document.getElementById("status_time");

let relay_lighting = document.getElementById("relay_lighting");
let relay_lighting_purple = document.getElementById("relay_lighting_purple");
let relay_air = document.getElementById("relay_air");
let relay_air_fish = document.getElementById("relay_air_fish");
let relay_water_out = document.getElementById("relay_water_out");
let relay_water_in = document.getElementById("relay_water_in");
let relay_temp = document.getElementById("relay_temp");
let gett_auqa_temp = document.getElementById("get_auqa_temp");

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

let timer_time_air_fish_m_off = document.getElementById(
	"timer_time_air_fish_m_off"
);
let timer_time_water_out_sec_off = document.getElementById(
	"timer_time_water_out_sec_off"
);
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

let tagBody = document.getElementsByTagName("body");

let setup_timer_time_lighting_purple_m_off = document.getElementById(
	"set_timer_time_lighting_purple_m_off"
);
let btn_set_timer_time_lighting_purple_m_off = document.getElementById(
	"btn_set_timer_time_lighting_purple_m_off"
);
let setup_timer_time_air_m_off = document.getElementById(
	"set_timer_time_air_m_off"
);
let btn_setup_timer_time_lighting_air_m_off = document.getElementById(
	"btn_set_timer_time_lighting_air_m_off"
);

let setup_timer_time_water_out_m_off = document.getElementById(
	"set_timer_time_water_out_m_off"
);
let btn_setup_timer_time_water_out_m_off = document.getElementById(
	"btn_set_timer_time_water_out_m_off"
);
let set_timer_time_water_out_sec_off = document.getElementById(
	"set_timer_time_water_out_sec_off"
);
let btn_set_timer_time_water_out_sec_off = document.getElementById(
	"btn_set_timer_time_water_out_sec_off"
);
let btn_set_timer_time_lighting_fish_air_m_off = document.getElementById(
	"btn_set_timer_time_lighting_fish_air_m_off"
);

let set_timer_fish_time_air_m_off = document.getElementById(
	"set_timer_fish_time_air_m_off"
);

let get_auqa_temp = document.getElementById("get_auqa_temp");
let set_auqa_temp = document.getElementById("set_auqa_temp");
let btn_setup_temp = document.getElementById("btn_setup_temp");
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

//Воздух рыбам
function relay_state_air_fish() {
	var request = new XMLHttpRequest();
	request.open("GET", "/relay_status_air_fish", true);
	request.onload = function () {
		if (request.readyState == 4 && request.status == 200) {
			var response = request.responseText;
			relay_status = Number.parseInt(response);
			if (relay_status == 0) {
				relay_air_fish.classList.add("relay_off");
			} else {
				relay_air_fish.classList.add("relay_on");
			}
		}
	};
	request.send();
}
//Воздух рыбам
function relay_inverse_air_fish() {
	var request = new XMLHttpRequest();
	request.open("GET", "/relay_switch_air_fish", false);
	request.send();
	if (request.readyState == 4 && request.status == 200) {
		var response = request.responseText;
		if (response == "0") {
			relay_air_fish.classList.remove("relay_on");
			relay_air_fish.classList.add("relay_off");
		} else {
			relay_air_fish.classList.remove("relay_off");
			relay_air_fish.classList.add("relay_on");
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

function setup_timer_time_min_purple() {
	var request = new XMLHttpRequest();
	value = Number.parseInt(setup_timer_time_lighting_purple_m_off.value);
	switch (value) {
		case 1:
			request.open("GET", "/setup_timer_time_min_1_purple", false);
			alert("Установлено!");
			break;
		case 2:
			request.open("GET", "/setup_timer_time_min_2_purple", false);
			alert("Установлено!");
			break;
		case 3:
			request.open("GET", "/setup_timer_time_min_3_purple", false);
			alert("Установлено!");
			break;
		case 4:
			request.open("GET", "/setup_timer_time_min_4_purple", false);
			alert("Установлено!");
			break;
		case 5:
			request.open("GET", "/setup_timer_time_min_5_purple", false);
			alert("Установлено!");
			break;
		case 6:
			request.open("GET", "/setup_timer_time_min_6_purple", false);
			alert("Установлено!");
			break;
		case 7:
			request.open("GET", "/setup_timer_time_min_7_purple", false);
			alert("Установлено!");
			break;
		case 8:
			request.open("GET", "/setup_timer_time_min_8_purple", false);
			alert("Установлено!");
			break;
		case 9:
			request.open("GET", "/setup_timer_time_min_9_purple", false);
			alert("Установлено!");
			break;
		case 10:
			request.open("GET", "/setup_timer_time_min_10_purple", false);
			alert("Установлено!");
			break;
		case 11:
			request.open("GET", "/setup_timer_time_min_11_purple", false);
			alert("Установлено!");
			break;
		case 12:
			request.open("GET", "/setup_timer_time_min_12_purple", false);
			alert("Установлено!");
			break;
		case 13:
			request.open("GET", "/setup_timer_time_min_13_purple", false);
			alert("Установлено!");
			break;
		case 14:
			request.open("GET", "/setup_timer_time_min_14_purple", false);
			alert("Установлено!");
			break;
		case 15:
			request.open("GET", "/setup_timer_time_min_15_purple", false);
			alert("Установлено!");
			break;
		case 16:
			request.open("GET", "/setup_timer_time_min_16_purple", false);
			alert("Установлено!");
			break;
		case 17:
			request.open("GET", "/setup_timer_time_min_17_purple", false);
			alert("Установлено!");
			break;
		case 18:
			request.open("GET", "/setup_timer_time_min_18_purple", false);
			alert("Установлено!");
			break;
		case 19:
			request.open("GET", "/setup_timer_time_min_19_purple", false);
			alert("Установлено!");
			break;
		case 20:
			request.open("GET", "/setup_timer_time_min_20_purple", false);
			alert("Установлено!");
			break;
		case 21:
			request.open("GET", "/setup_timer_time_min_21_purple", false);
			alert("Установлено!");
			break;
		case 22:
			request.open("GET", "/setup_timer_time_min_22_purple", false);
			alert("Установлено!");
			break;
		case 23:
			request.open("GET", "/setup_timer_time_min_23_purple", false);
			alert("Установлено!");
			break;
		case 24:
			request.open("GET", "/setup_timer_time_min_24_purple", false);
			alert("Установлено!");
			break;
		case 25:
			request.open("GET", "/setup_timer_time_min_25_purple", false);
			alert("Установлено!");
			break;
		case 26:
			request.open("GET", "/setup_timer_time_min_26_purple", false);
			alert("Установлено!");
			break;
		case 27:
			request.open("GET", "/setup_timer_time_min_27_purple", false);
			alert("Установлено!");
			break;
		case 28:
			request.open("GET", "/setup_timer_time_min_28_purple", false);
			alert("Установлено!");
			break;
		case 29:
			request.open("GET", "/setup_timer_time_min_29_purple", false);
			alert("Установлено!");
			break;
		case 30:
			request.open("GET", "/setup_timer_time_min_30_purple", false);
			alert("Установлено!");
			break;
		case 31:
			request.open("GET", "/setup_timer_time_min_31_purple", false);
			alert("Установлено!");
			break;
		case 32:
			request.open("GET", "/setup_timer_time_min_32_purple", false);
			alert("Установлено!");
			break;
		case 33:
			request.open("GET", "/setup_timer_time_min_33_purple", false);
			alert("Установлено!");
			break;
		case 34:
			request.open("GET", "/setup_timer_time_min_34_purple", false);
			alert("Установлено!");
			break;
		case 35:
			request.open("GET", "/setup_timer_time_min_35_purple", false);
			alert("Установлено!");
			break;
		case 36:
			request.open("GET", "/setup_timer_time_min_36_purple", false);
			alert("Установлено!");
			break;
		case 37:
			request.open("GET", "/setup_timer_time_min_37_purple", false);
			alert("Установлено!");
			break;
		case 38:
			request.open("GET", "/setup_timer_time_min_38_purple", false);
			alert("Установлено!");
			break;
		case 39:
			request.open("GET", "/setup_timer_time_min_39_purple", false);
			alert("Установлено!");
			break;
		case 40:
			request.open("GET", "/setup_timer_time_min_40_purple", false);
			alert("Установлено!");
			break;
		case 41:
			request.open("GET", "/setup_timer_time_min_41_purple", false);
			alert("Установлено!");
			break;
		case 42:
			request.open("GET", "/setup_timer_time_min_42_purple", false);
			alert("Установлено!");
			break;
		case 43:
			request.open("GET", "/setup_timer_time_min_43_purple", false);
			alert("Установлено!");
			break;
		case 44:
			request.open("GET", "/setup_timer_time_min_44_purple", false);
			alert("Установлено!");
			break;
		case 45:
			request.open("GET", "/setup_timer_time_min_45_purple", false);
			alert("Установлено!");
			break;
		case 46:
			request.open("GET", "/setup_timer_time_min_46_purple", false);
			alert("Установлено!");
			break;
		case 47:
			request.open("GET", "/setup_timer_time_min_47_purple", false);
			alert("Установлено!");
			break;
		case 48:
			request.open("GET", "/setup_timer_time_min_48_purple", false);
			alert("Установлено!");
			break;
		case 49:
			request.open("GET", "/setup_timer_time_min_49_purple", false);
			alert("Установлено!");
			break;
		case 50:
			request.open("GET", "/setup_timer_time_min_50_purple", false);
			alert("Установлено!");
			break;
		case 51:
			request.open("GET", "/setup_timer_time_min_51_purple", false);
			alert("Установлено!");
			break;
		case 52:
			request.open("GET", "/setup_timer_time_min_52_purple", false);
			alert("Установлено!");
			break;
		case 53:
			request.open("GET", "/setup_timer_time_min_53_purple", false);
			alert("Установлено!");
			break;
		case 54:
			request.open("GET", "/setup_timer_time_min_54_purple", false);
			alert("Установлено!");
			break;
		case 55:
			request.open("GET", "/setup_timer_time_min_55_purple", false);
			alert("Установлено!");
			break;
		case 56:
			request.open("GET", "/setup_timer_time_min_56_purple", false);
			alert("Установлено!");
			break;
		case 57:
			request.open("GET", "/setup_timer_time_min_57_purple", false);
			alert("Установлено!");
			break;
		case 58:
			request.open("GET", "/setup_timer_time_min_58_purple", false);
			alert("Установлено!");
			break;
		case 59:
			request.open("GET", "/setup_timer_time_min_59_purple", false);
			alert("Установлено!");
			break;
		case 60:
			request.open("GET", "/setup_timer_time_min_60_purple", false);
			alert("Установлено!");
			break;
		default:
			alert("Недопустимое значение!");
			break;
	}
	request.send();
	location.reload();
}
function setup_timer_time_lighting_air_m_off() {
	var request = new XMLHttpRequest();
	value = Number.parseInt(setup_timer_time_air_m_off.value);
	switch (value) {
		case 1:
			request.open("GET", "/setup_timer_time_min_1_air", false);
			alert("Установлено!");
			break;
		case 2:
			request.open("GET", "/setup_timer_time_min_2_air", false);
			alert("Установлено!");
			break;
		case 3:
			request.open("GET", "/setup_timer_time_min_3_air", false);
			alert("Установлено!");
			break;
		case 4:
			request.open("GET", "/setup_timer_time_min_4_air", false);
			alert("Установлено!");
			break;
		case 5:
			request.open("GET", "/setup_timer_time_min_5_air", false);
			alert("Установлено!");
			break;
		case 6:
			request.open("GET", "/setup_timer_time_min_6_air", false);
			alert("Установлено!");
			break;
		case 7:
			request.open("GET", "/setup_timer_time_min_7_air", false);
			alert("Установлено!");
			break;
		case 8:
			request.open("GET", "/setup_timer_time_min_8_air", false);
			alert("Установлено!");
			break;
		case 9:
			request.open("GET", "/setup_timer_time_min_9_air", false);
			alert("Установлено!");
			break;
		case 10:
			request.open("GET", "/setup_timer_time_min_10_air", false);
			alert("Установлено!");
			break;
		case 11:
			request.open("GET", "/setup_timer_time_min_11_air", false);
			alert("Установлено!");
			break;
		case 12:
			request.open("GET", "/setup_timer_time_min_12_air", false);
			alert("Установлено!");
			break;
		case 13:
			request.open("GET", "/setup_timer_time_min_13_air", false);
			alert("Установлено!");
			break;
		case 14:
			request.open("GET", "/setup_timer_time_min_14_air", false);
			alert("Установлено!");
			break;
		case 15:
			request.open("GET", "/setup_timer_time_min_15_air", false);
			alert("Установлено!");
			break;
		case 16:
			request.open("GET", "/setup_timer_time_min_16_air", false);
			alert("Установлено!");
			break;
		case 17:
			request.open("GET", "/setup_timer_time_min_17_air", false);
			alert("Установлено!");
			break;
		case 18:
			request.open("GET", "/setup_timer_time_min_18_air", false);
			alert("Установлено!");
			break;
		case 19:
			request.open("GET", "/setup_timer_time_min_19_air", false);
			alert("Установлено!");
			break;
		case 20:
			request.open("GET", "/setup_timer_time_min_20_air", false);
			alert("Установлено!");
			break;
		case 21:
			request.open("GET", "/setup_timer_time_min_21_air", false);
			alert("Установлено!");
			break;
		case 22:
			request.open("GET", "/setup_timer_time_min_22_air", false);
			alert("Установлено!");
			break;
		case 23:
			request.open("GET", "/setup_timer_time_min_23_air", false);
			alert("Установлено!");
			break;
		case 24:
			request.open("GET", "/setup_timer_time_min_24_air", false);
			alert("Установлено!");
			break;
		case 25:
			request.open("GET", "/setup_timer_time_min_25_air", false);
			alert("Установлено!");
			break;
		case 26:
			request.open("GET", "/setup_timer_time_min_26_air", false);
			alert("Установлено!");
			break;
		case 27:
			request.open("GET", "/setup_timer_time_min_27_air", false);
			alert("Установлено!");
			break;
		case 28:
			request.open("GET", "/setup_timer_time_min_28_air", false);
			alert("Установлено!");
			break;
		case 29:
			request.open("GET", "/setup_timer_time_min_29_air", false);
			alert("Установлено!");
			break;
		case 30:
			request.open("GET", "/setup_timer_time_min_30_air", false);
			alert("Установлено!");
			break;
		case 31:
			request.open("GET", "/setup_timer_time_min_31_air", false);
			alert("Установлено!");
			break;
		case 32:
			request.open("GET", "/setup_timer_time_min_32_air", false);
			alert("Установлено!");
			break;
		case 33:
			request.open("GET", "/setup_timer_time_min_33_air", false);
			alert("Установлено!");
			break;
		case 34:
			request.open("GET", "/setup_timer_time_min_34_air", false);
			alert("Установлено!");
			break;
		case 35:
			request.open("GET", "/setup_timer_time_min_35_air", false);
			alert("Установлено!");
			break;
		case 36:
			request.open("GET", "/setup_timer_time_min_36_air", false);
			alert("Установлено!");
			break;
		case 37:
			request.open("GET", "/setup_timer_time_min_37_air", false);
			alert("Установлено!");
			break;
		case 38:
			request.open("GET", "/setup_timer_time_min_38_air", false);
			alert("Установлено!");
			break;
		case 39:
			request.open("GET", "/setup_timer_time_min_39_air", false);
			alert("Установлено!");
			break;
		case 40:
			request.open("GET", "/setup_timer_time_min_40_air", false);
			alert("Установлено!");
			break;
		case 41:
			request.open("GET", "/setup_timer_time_min_41_air", false);
			alert("Установлено!");
			break;
		case 42:
			request.open("GET", "/setup_timer_time_min_42_air", false);
			alert("Установлено!");
			break;
		case 43:
			request.open("GET", "/setup_timer_time_min_43_air", false);
			alert("Установлено!");
			break;
		case 44:
			request.open("GET", "/setup_timer_time_min_44_air", false);
			alert("Установлено!");
			break;
		case 45:
			request.open("GET", "/setup_timer_time_min_45_air", false);
			alert("Установлено!");
			break;
		case 46:
			request.open("GET", "/setup_timer_time_min_46_air", false);
			alert("Установлено!");
			break;
		case 47:
			request.open("GET", "/setup_timer_time_min_47_air", false);
			alert("Установлено!");
			break;
		case 48:
			request.open("GET", "/setup_timer_time_min_48_air", false);
			alert("Установлено!");
			break;
		case 49:
			request.open("GET", "/setup_timer_time_min_49_air", false);
			alert("Установлено!");
		case 50:
			request.open("GET", "/setup_timer_time_min_50_air", false);
			alert("Установлено!");
			break;
		case 51:
			request.open("GET", "/setup_timer_time_min_51_air", false);
			alert("Установлено!");
			break;
		case 52:
			request.open("GET", "/setup_timer_time_min_52_air", false);
			alert("Установлено!");
			break;
		case 53:
			request.open("GET", "/setup_timer_time_min_53_air", false);
			alert("Установлено!");
			break;
		case 54:
			request.open("GET", "/setup_timer_time_min_54_air", false);
			alert("Установлено!");
			break;
		case 55:
			request.open("GET", "/setup_timer_time_min_55_air", false);
			alert("Установлено!");
			break;
		case 56:
			request.open("GET", "/setup_timer_time_min_56_air", false);
			alert("Установлено!");
			break;
		case 57:
			request.open("GET", "/setup_timer_time_min_57_air", false);
			alert("Установлено!");
			break;
		case 58:
			request.open("GET", "/setup_timer_time_min_58_air", false);
			alert("Установлено!");
			break;
		case 59:
			request.open("GET", "/setup_timer_time_min_59_air", false);
			alert("Установлено!");
			break;
		case 60:
			request.open("GET", "/setup_timer_time_min_60_air", false);
			alert("Установлено!");
			break;
		default:
			alert("Недопустимое значение!");
			break;
	}
	request.send();
	location.reload();
}
function setup_timer_time_lighting_air_m_off_fish() {
	var request = new XMLHttpRequest();
	value = Number.parseInt(set_timer_fish_time_air_m_off.value);
	switch (value) {
		case 1:
			request.open("GET", "/setup_timer_time_min_1_air_fish", false);
			alert("Установлено!");
			break;
		case 2:
			request.open("GET", "/setup_timer_time_min_2_air_fish", false);
			alert("Установлено!");
			break;
		case 3:
			request.open("GET", "/setup_timer_time_min_3_air_fish", false);
			alert("Установлено!");
			break;
		case 4:
			request.open("GET", "/setup_timer_time_min_4_air_fish", false);
			alert("Установлено!");
			break;
		case 5:
			request.open("GET", "/setup_timer_time_min_5_air_fish", false);
			alert("Установлено!");
			break;
		case 6:
			request.open("GET", "/setup_timer_time_min_6_air_fish", false);
			alert("Установлено!");
			break;
		case 7:
			request.open("GET", "/setup_timer_time_min_7_air_fish", false);
			alert("Установлено!");
			break;
		case 8:
			request.open("GET", "/setup_timer_time_min_8_air_fish", false);
			alert("Установлено!");
			break;
		case 9:
			request.open("GET", "/setup_timer_time_min_9_air_fish", false);
			alert("Установлено!");
			break;
		case 10:
			request.open("GET", "/setup_timer_time_min_10_air_fish", false);
			alert("Установлено!");
			break;
		case 11:
			request.open("GET", "/setup_timer_time_min_11_air_fish", false);
			alert("Установлено!");
			break;
		case 12:
			request.open("GET", "/setup_timer_time_min_12_air_fish", false);
			alert("Установлено!");
			break;
		case 13:
			request.open("GET", "/setup_timer_time_min_13_air_fish", false);
			alert("Установлено!");
			break;
		case 14:
			request.open("GET", "/setup_timer_time_min_14_air_fish", false);
			alert("Установлено!");
			break;
		case 15:
			request.open("GET", "/setup_timer_time_min_15_air_fish", false);
			alert("Установлено!");
			break;
		case 16:
			request.open("GET", "/setup_timer_time_min_16_air_fish", false);
			alert("Установлено!");
			break;
		case 17:
			request.open("GET", "/setup_timer_time_min_17_air_fish", false);
			alert("Установлено!");
			break;
		case 18:
			request.open("GET", "/setup_timer_time_min_18_air_fish", false);
			alert("Установлено!");
			break;
		case 19:
			request.open("GET", "/setup_timer_time_min_19_air_fish", false);
			alert("Установлено!");
			break;
		case 20:
			request.open("GET", "/setup_timer_time_min_20_air_fish", false);
			alert("Установлено!");
			break;
		case 21:
			request.open("GET", "/setup_timer_time_min_21_air_fish", false);
			alert("Установлено!");
			break;
		case 22:
			request.open("GET", "/setup_timer_time_min_22_air_fish", false);
			alert("Установлено!");
			break;
		case 23:
			request.open("GET", "/setup_timer_time_min_23_air_fish", false);
			alert("Установлено!");
			break;
		case 24:
			request.open("GET", "/setup_timer_time_min_24_air_fish", false);
			alert("Установлено!");
			break;
		case 25:
			request.open("GET", "/setup_timer_time_min_25_air_fish", false);
			alert("Установлено!");
			break;
		case 26:
			request.open("GET", "/setup_timer_time_min_26_air_fish", false);
			alert("Установлено!");
			break;
		case 27:
			request.open("GET", "/setup_timer_time_min_27_air_fish", false);
			alert("Установлено!");
			break;
		case 28:
			request.open("GET", "/setup_timer_time_min_28_air_fish", false);
			alert("Установлено!");
			break;
		case 29:
			request.open("GET", "/setup_timer_time_min_29_air_fish", false);
			alert("Установлено!");
			break;
		case 30:
			request.open("GET", "/setup_timer_time_min_30_air_fish", false);
			alert("Установлено!");
			break;
		case 31:
			request.open("GET", "/setup_timer_time_min_31_air_fish", false);
			alert("Установлено!");
			break;
		case 32:
			request.open("GET", "/setup_timer_time_min_32_air_fish", false);
			alert("Установлено!");
			break;
		case 33:
			request.open("GET", "/setup_timer_time_min_33_air_fish", false);
			alert("Установлено!");
			break;
		case 34:
			request.open("GET", "/setup_timer_time_min_34_air_fish", false);
			alert("Установлено!");
			break;
		case 35:
			request.open("GET", "/setup_timer_time_min_35_air_fish", false);
			alert("Установлено!");
			break;
		case 36:
			request.open("GET", "/setup_timer_time_min_36_air_fish", false);
			alert("Установлено!");
			break;
		case 37:
			request.open("GET", "/setup_timer_time_min_37_air_fish", false);
			alert("Установлено!");
			break;
		case 38:
			request.open("GET", "/setup_timer_time_min_38_air_fish", false);
			alert("Установлено!");
			break;
		case 39:
			request.open("GET", "/setup_timer_time_min_39_air_fish", false);
			alert("Установлено!");
			break;
		case 40:
			request.open("GET", "/setup_timer_time_min_40_air_fish", false);
			alert("Установлено!");
			break;
		case 41:
			request.open("GET", "/setup_timer_time_min_41_air_fish", false);
			alert("Установлено!");
			break;
		case 42:
			request.open("GET", "/setup_timer_time_min_42_air_fish", false);
			alert("Установлено!");
			break;
		case 43:
			request.open("GET", "/setup_timer_time_min_43_air_fish", false);
			alert("Установлено!");
			break;
		case 44:
			request.open("GET", "/setup_timer_time_min_44_air_fish", false);
			alert("Установлено!");
			break;
		case 45:
			request.open("GET", "/setup_timer_time_min_45_air_fish", false);
			alert("Установлено!");
			break;
		case 46:
			request.open("GET", "/setup_timer_time_min_46_air_fish", false);
			alert("Установлено!");
			break;
		case 47:
			request.open("GET", "/setup_timer_time_min_47_air_fish", false);
			alert("Установлено!");
			break;
		case 48:
			request.open("GET", "/setup_timer_time_min_48_air_fish", false);
			alert("Установлено!");
			break;
		case 49:
			request.open("GET", "/setup_timer_time_min_49_air_fish", false);
			alert("Установлено!");
		case 50:
			request.open("GET", "/setup_timer_time_min_50_air_fish", false);
			alert("Установлено!");
			break;
		case 51:
			request.open("GET", "/setup_timer_time_min_51_air_fish", false);
			alert("Установлено!");
			break;
		case 52:
			request.open("GET", "/setup_timer_time_min_52_air_fish", false);
			alert("Установлено!");
			break;
		case 53:
			request.open("GET", "/setup_timer_time_min_53_air_fish", false);
			alert("Установлено!");
			break;
		case 54:
			request.open("GET", "/setup_timer_time_min_54_air_fish", false);
			alert("Установлено!");
			break;
		case 55:
			request.open("GET", "/setup_timer_time_min_55_air_fish", false);
			alert("Установлено!");
			break;
		case 56:
			request.open("GET", "/setup_timer_time_min_56_air_fish", false);
			alert("Установлено!");
			break;
		case 57:
			request.open("GET", "/setup_timer_time_min_57_air_fish", false);
			alert("Установлено!");
			break;
		case 58:
			request.open("GET", "/setup_timer_time_min_58_air_fish", false);
			alert("Установлено!");
			break;
		case 59:
			request.open("GET", "/setup_timer_time_min_59_air_fish", false);
			alert("Установлено!");
			break;
		case 60:
			request.open("GET", "/setup_timer_time_min_60_air_fish", false);
			alert("Установлено!");
			break;
		default:
			alert("Недопустимое значение!");
			break;
	}
	request.send();
	location.reload();
}
function setup_timer_time_lighting_water_out() {
	var request = new XMLHttpRequest();
	value = Number.parseInt(setup_timer_time_water_out_m_off.value);
	switch (value) {
		case 1:
			request.open("GET", "/setup_timer_time_min_1_water", false);
			alert("Установлено!");
			break;
		case 2:
			request.open("GET", "/setup_timer_time_min_2_water", false);
			alert("Установлено!");
			break;
		case 3:
			request.open("GET", "/setup_timer_time_min_3_water", false);
			alert("Установлено!");
			break;
		case 4:
			request.open("GET", "/setup_timer_time_min_4_water", false);
			alert("Установлено!");
			break;
		case 5:
			request.open("GET", "/setup_timer_time_min_5_water", false);
			alert("Установлено!");
			break;
		case 6:
			request.open("GET", "/setup_timer_time_min_6_water", false);
			alert("Установлено!");
			break;
		case 7:
			request.open("GET", "/setup_timer_time_min_7_water", false);
			alert("Установлено!");
			break;
		case 8:
			request.open("GET", "/setup_timer_time_min_8_water", false);
			alert("Установлено!");
			break;
		case 9:
			request.open("GET", "/setup_timer_time_min_9_water", false);
			alert("Установлено!");
			break;
		case 10:
			request.open("GET", "/setup_timer_time_min_10_water", false);
			alert("Установлено!");
			break;
		case 11:
			request.open("GET", "/setup_timer_time_min_11_water", false);
			alert("Установлено!");
			break;
		case 12:
			request.open("GET", "/setup_timer_time_min_12_water", false);
			alert("Установлено!");
			break;
		case 13:
			request.open("GET", "/setup_timer_time_min_13_water", false);
			alert("Установлено!");
			break;
		case 14:
			request.open("GET", "/setup_timer_time_min_14_water", false);
			alert("Установлено!");
			break;
		case 15:
			request.open("GET", "/setup_timer_time_min_15_water", false);
			alert("Установлено!");
			break;
		case 16:
			request.open("GET", "/setup_timer_time_min_16_water", false);
			alert("Установлено!");
			break;
		case 17:
			request.open("GET", "/setup_timer_time_min_17_water", false);
			alert("Установлено!");
			break;
		case 18:
			request.open("GET", "/setup_timer_time_min_18_water", false);
			alert("Установлено!");
			break;
		case 19:
			request.open("GET", "/setup_timer_time_min_19_water", false);
			alert("Установлено!");
			break;
		case 20:
			request.open("GET", "/setup_timer_time_min_20_water", false);
			alert("Установлено!");
			break;
		case 21:
			request.open("GET", "/setup_timer_time_min_21_water", false);
			alert("Установлено!");
			break;
		case 22:
			request.open("GET", "/setup_timer_time_min_22_water", false);
			alert("Установлено!");
			break;
		case 23:
			request.open("GET", "/setup_timer_time_min_23_water", false);
			alert("Установлено!");
			break;
		case 24:
			request.open("GET", "/setup_timer_time_min_24_water", false);
			alert("Установлено!");
			break;
		case 25:
			request.open("GET", "/setup_timer_time_min_25_water", false);
			alert("Установлено!");
			break;
		case 26:
			request.open("GET", "/setup_timer_time_min_26_water", false);
			alert("Установлено!");
			break;
		case 27:
			request.open("GET", "/setup_timer_time_min_27_water", false);
			alert("Установлено!");
			break;
		case 28:
			request.open("GET", "/setup_timer_time_min_28_water", false);
			alert("Установлено!");
			break;
		case 29:
			request.open("GET", "/setup_timer_time_min_29_water", false);
			alert("Установлено!");
			break;
		case 30:
			request.open("GET", "/setup_timer_time_min_30_water", false);
			alert("Установлено!");
			break;
		case 31:
			request.open("GET", "/setup_timer_time_min_31_water", false);
			alert("Установлено!");
			break;
		case 32:
			request.open("GET", "/setup_timer_time_min_32_water", false);
			alert("Установлено!");
			break;
		case 33:
			request.open("GET", "/setup_timer_time_min_33_water", false);
			alert("Установлено!");
			break;
		case 34:
			request.open("GET", "/setup_timer_time_min_34_water", false);
			alert("Установлено!");
			break;
		case 35:
			request.open("GET", "/setup_timer_time_min_35_water", false);
			alert("Установлено!");
			break;
		case 36:
			request.open("GET", "/setup_timer_time_min_36_water", false);
			alert("Установлено!");
			break;
		case 37:
			request.open("GET", "/setup_timer_time_min_37_water", false);
			alert("Установлено!");
			break;
		case 38:
			request.open("GET", "/setup_timer_time_min_38_water", false);
			alert("Установлено!");
			break;
		case 39:
			request.open("GET", "/setup_timer_time_min_39_water", false);
			alert("Установлено!");
			break;
		case 40:
			request.open("GET", "/setup_timer_time_min_40_water", false);
			alert("Установлено!");
			break;
		case 41:
			request.open("GET", "/setup_timer_time_min_41_water", false);
			alert("Установлено!");
			break;
		case 42:
			request.open("GET", "/setup_timer_time_min_42_water", false);
			alert("Установлено!");
			break;
		case 43:
			request.open("GET", "/setup_timer_time_min_43_water", false);
			alert("Установлено!");
			break;
		case 44:
			request.open("GET", "/setup_timer_time_min_44_water", false);
			alert("Установлено!");
			break;
		case 45:
			request.open("GET", "/setup_timer_time_min_45_water", false);
			alert("Установлено!");
			break;
		case 46:
			request.open("GET", "/setup_timer_time_min_46_water", false);
			alert("Установлено!");
			break;
		case 47:
			request.open("GET", "/setup_timer_time_min_47_water", false);
			alert("Установлено!");
			break;
		case 48:
			request.open("GET", "/setup_timer_time_min_48_water", false);
			alert("Установлено!");
			break;
		case 49:
			request.open("GET", "/setup_timer_time_min_49_water", false);
			alert("Установлено!");
		case 50:
			request.open("GET", "/setup_timer_time_min_50_water", false);
			alert("Установлено!");
			break;
		case 51:
			request.open("GET", "/setup_timer_time_min_51_water", false);
			alert("Установлено!");
			break;
		case 52:
			request.open("GET", "/setup_timer_time_min_52_water", false);
			alert("Установлено!");
			break;
		case 53:
			request.open("GET", "/setup_timer_time_min_53_water", false);
			alert("Установлено!");
			break;
		case 54:
			request.open("GET", "/setup_timer_time_min_54_water", false);
			alert("Установлено!");
			break;
		case 55:
			request.open("GET", "/setup_timer_time_min_55_water", false);
			alert("Установлено!");
			break;
		case 56:
			request.open("GET", "/setup_timer_time_min_56_water", false);
			alert("Установлено!");
			break;
		case 57:
			request.open("GET", "/setup_timer_time_min_57_water", false);
			alert("Установлено!");
			break;
		case 58:
			request.open("GET", "/setup_timer_time_min_58_water", false);
			alert("Установлено!");
			break;
		case 59:
			request.open("GET", "/setup_timer_time_min_59_water", false);
			alert("Установлено!");
			break;
		case 60:
			request.open("GET", "/setup_timer_time_min_60_water", false);
			alert("Установлено!");
			break;
		default:
			alert("Недопустимое значение!");
			break;
	}
	request.send();
	location.reload();
}

function setup_temp() {
	var request = new XMLHttpRequest();
	value = Number.parseInt(set_auqa_temp.value);
	switch (value) {
		case 10:
			request.open("GET", "/setup_temp_10", false);
			alert("Установлено!");
			break;
		case 11:
			request.open("GET", "/setup_temp_11", false);
			alert("Установлено!");
			break;
		case 12:
			request.open("GET", "/setup_temp_12", false);
			alert("Установлено!");
			break;
		case 13:
			request.open("GET", "/setup_temp_13", false);
			alert("Установлено!");
			break;
		case 14:
			request.open("GET", "/setup_temp_14", false);
			alert("Установлено!");
			break;
		case 15:
			request.open("GET", "/setup_temp_15", false);
			alert("Установлено!");
			break;
		case 16:
			request.open("GET", "/setup_temp_16", false);
			alert("Установлено!");
			break;
		case 17:
			request.open("GET", "/setup_temp_17", false);
			alert("Установлено!");
			break;
		case 18:
			request.open("GET", "/setup_temp_18", false);
			alert("Установлено!");
			break;
		case 19:
			request.open("GET", "/setup_temp_19", false);
			alert("Установлено!");
			break;
		case 20:
			request.open("GET", "/setup_temp_20", false);
			alert("Установлено!");
			break;
		case 21:
			request.open("GET", "/setup_temp_21", false);
			alert("Установлено!");
			break;
		case 22:
			request.open("GET", "/setup_temp_22", false);
			alert("Установлено!");
			break;
		case 23:
			request.open("GET", "/setup_temp_23", false);
			alert("Установлено!");
			break;
		case 24:
			request.open("GET", "/setup_temp_24", false);
			alert("Установлено!");
			break;
		case 25:
			request.open("GET", "/setup_temp_25", false);
			alert("Установлено!");
			break;
		case 26:
			request.open("GET", "/setup_temp_26", false);
			alert("Установлено!");
			break;
		case 27:
			request.open("GET", "/setup_temp_27", false);
			alert("Установлено!");
			break;
		case 28:
			request.open("GET", "/setup_temp_28", false);
			alert("Установлено!");
			break;
		case 29:
			request.open("GET", "/setup_temp_29", false);
			alert("Установлено!");
			break;
		case 30:
			request.open("GET", "/setup_temp_30", false);
			alert("Установлено!");
			break;
		case 31:
			request.open("GET", "/setup_temp_31", false);
			alert("Установлено!");
			break;
		case 32:
			request.open("GET", "/setup_temp_32", false);
			alert("Установлено!");
			break;
		case 33:
			request.open("GET", "/setup_temp_33", false);
			alert("Установлено!");
			break;
		case 34:
			request.open("GET", "/setup_temp_34", false);
			alert("Установлено!");
			break;
		default:
			alert("Недопустимое значение!(От 10 градусов до 34!)");
			break;
	}
	request.send();
	location.reload();
}

function setup_timer_time_water_in_m_off() {
	var request = new XMLHttpRequest();
	value = Number.parseInt(set_timer_time_water_out_sec_off.value);
	switch (value) {
		case 5:
			request.open("GET", "/setup_timer_time_min_5_water_sec", false);
			alert("Установлено!");
			break;
		case 10:
			request.open("GET", "/setup_timer_time_min_10_water_sec", false);
			alert("Установлено!");
			break;
		case 15:
			request.open("GET", "/setup_timer_time_min_15_water_sec", false);
			alert("Установлено!");
			break;
		case 20:
			request.open("GET", "/setup_timer_time_min_20_water_sec", false);
			alert("Установлено!");
			break;
		case 25:
			request.open("GET", "/setup_timer_time_min_25_water_sec", false);
			alert("Установлено!");
			break;
		case 30:
			request.open("GET", "/setup_timer_time_min_30_water_sec", false);
			alert("Установлено!");
			break;
		case 35:
			request.open("GET", "/setup_timer_time_min_35_water_sec", false);
			alert("Установлено!");
			break;
		case 40:
			request.open("GET", "/setup_timer_time_min_40_water_sec", false);
			alert("Установлено!");
			break;
		case 45:
			request.open("GET", "/setup_timer_time_min_45_water_sec", false);
			alert("Установлено!");
			break;
		case 50:
			request.open("GET", "/setup_timer_time_min_50_water_sec", false);
			alert("Установлено!");
			break;
		case 55:
			request.open("GET", "/setup_timer_time_min_55_water_sec", false);
			alert("Установлено!");
			break;
		case 60:
			request.open("GET", "/setup_timer_time_min_60_water_sec", false);
			alert("Установлено!");
			break;
		default:
			alert("Недопустимое значение!(5,10,15,20,25,30 и т.д)");
			break;
	}
	request.send();
	location.reload();
}

//получение температуры
function auqa_temp() {
	var request = new XMLHttpRequest();
	request.open("GET", "/aqua_temp", false);
	request.onload = function () {
		if (request.status == 200) {
			temp.value = request.responseText;
		} else temp.value = 0;
	};
	request.send();
}
function g_auqa_temp() {
	var request = new XMLHttpRequest();
	request.open("GET", "/get_aqua_temp", false);
	request.onload = function () {
		if (request.status == 200) {
			gett_auqa_temp.value = request.responseText;
		} else gett_auqa_temp.value = 0;
	};
	request.send();
}
//получение времени
function status_time() {
	var request = new XMLHttpRequest();
	request.open("GET", "/status_time", false);
	request.onload = function () {
		if (request.status == 200) {
			time.value = request.responseText;
		} else time.value = "0";
	};
	request.send();
}

//получение времени
function timer_time_purple() {
	var request6 = new XMLHttpRequest();
	request6.open("GET", "/timer_time_lighting_purple_m_off", true);
	request6.onload = function () {
		//Ультрафиолет
		timer_time_ligh_purple_m_off.value = request6.responseText;
	};
	request6.send();
}
function timer_time_air() {
	var request8 = new XMLHttpRequest();
	request8.open("GET", "/timer_air_m_off", true);
	request8.onload = function () {
		//Воздух
		timer_air_m_off.value = request8.responseText;
	};
	request8.send();
}

function timer_time_water_in_sec() {
	var request = new XMLHttpRequest();
	request.open("GET", "/timer_time_water_in_sec", true);
	request.onload = function () {
		timer_time_water_out_sec_off.value = request.responseText;
	};
	request.send();
}

function timer_time_water() {
	var request10 = new XMLHttpRequest();
	//Вода из аквариума к растениям
	request10.open("GET", "/timer_time_water_out_m_off", true);
	request10.onload = function () {
		//Вода из аквариума к растениям и вода от растений в аквариум
		timer_water_out_m_off.value = request10.responseText;
	};
	request10.send();
}

document.addEventListener("DOMContentLoaded", relay_state_lighting);
document.addEventListener("DOMContentLoaded", relay_state_lighting_purple);
document.addEventListener("DOMContentLoaded", relay_state_air);
document.addEventListener("DOMContentLoaded", relay_state_air_fish);
document.addEventListener("DOMContentLoaded", relay_state_water_out);
document.addEventListener("DOMContentLoaded", relay_state_water_in);
document.addEventListener("DOMContentLoaded", relay_state_temp);
document.addEventListener("DOMContentLoaded", auqa_temp);
document.addEventListener("DOMContentLoaded", status_time);
document.addEventListener("DOMContentLoaded", timer_time_purple);

document.addEventListener("DOMContentLoaded", timer_time_air);
document.addEventListener("DOMContentLoaded", timer_time_water);
document.addEventListener("DOMContentLoaded", timer_time_water_in_sec);

document.addEventListener("DOMContentLoaded", g_auqa_temp);

relay_lighting.addEventListener("click", relay_inverse_lighting);
relay_lighting_purple.addEventListener("click", relay_inverse_lighting_purple);
relay_air.addEventListener("click", relay_inverse_air);
relay_air_fish.addEventListener("click", relay_inverse_air_fish);
relay_water_out.addEventListener("click", relay_inverse_water_out);
relay_water_in.addEventListener("click", relay_inverse_water_in);
relay_temp.addEventListener("click", relay_inverse_temp);
btn_set_timer_time_lighting_purple_m_off.addEventListener(
	"click",
	setup_timer_time_min_purple
);
btn_setup_timer_time_lighting_air_m_off.addEventListener(
	"click",
	setup_timer_time_lighting_air_m_off
);

btn_setup_temp.addEventListener("click", setup_temp);

btn_set_timer_time_water_out_sec_off.addEventListener(
	"click",
	setup_timer_time_water_in_m_off
);
btn_setup_timer_time_water_out_m_off.addEventListener(
	"click",
	setup_timer_time_lighting_water_out
);
