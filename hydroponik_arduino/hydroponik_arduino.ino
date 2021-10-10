#include <OneWire.h>
#include <DallasTemperature.h>
#include "DHT.h"
#include "SoftwareSerial.h"
#include  <LiquidCrystal_I2C.h>


LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);
//variabel serial (Rx, Tx)
SoftwareSerial DataSerial(0, 1);

// sensor ph di pin A2
#define ONE_WIRE_BUS A2

// setup sensor
OneWire oneWire(ONE_WIRE_BUS);


// berikan nama variabel,masukkan ke pustaka Dallas
DallasTemperature sensorSuhu(&oneWire);

#define DHTPIN A1
#define DHTTYPE DHT11
#define echoPin 11 //Echo Pin ultra sonik
#define trigPin 12 //Trigger Pin ultra sonik

DHT dht(DHTPIN, DHTTYPE);


int R1 = 1000;
int Ra = 25;

int ECPower = 7;
int ECPin = A3;


const unsigned long eventInterval = 3000;
unsigned long previousTime = 0;

float ppm_conversion = 0.5;
float Temp_Coef = 0.019;

float K = 1.6;
float constanta;

float temp1;
float Temperature = 27;
float EC = 0;
float EC25 = 0;
int ppm = 0;

int ppm_input;

float voltage = 0;
float Vref = 5;
float Vdrop = 0;
float Rc = 0;


float calibration_value = 21.34 - 0.7;
int phval = 0;
unsigned long int avgval;
int buffer_arr[10], temp;

float ph_act;

int s_dht_suhu = 0;
int s_dht_kelembapan = 0;
int s_suhu_air = 0;
float s_tds_sensor = 0;
float s_ph_sensor = 0;
int s_volume_air = 0;


String arrData[2];
int looping_lcd = 1;

void setup()
{
  Serial.begin(9600);
  DataSerial.begin(9600);
  lcd.begin(16, 2);
  lcd.backlight();
  delay(250);

  dht.begin();
  sensorSuhu.begin();
  pinMode(trigPin, OUTPUT); // Menjadikan trigPin menjadi OUTPUT
  pinMode(echoPin, INPUT); // Menjadikan echoPin menjadi INPUT

  pinMode(ECPin, INPUT);
  pinMode(ECPower, OUTPUT);

  delay(1000);

  R1 = (R1 + Ra);

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("OTOMATISASI");
  delay(1000);
  lcd.setCursor(4, 1);
  lcd.print("HYDROPONIK");
  delay(1000);

}

void loop() {

  s_dht_suhu = int(dht_suhu());
  s_dht_kelembapan = int(dht_kelembapan());
  s_suhu_air = int(suhu_air());
  s_tds_sensor = float(tds_sensor());
  s_ph_sensor = float(ph_sensor());
  s_volume_air =  int(volume_air());

  if (looping_lcd == 0) {
    lcd_pertama();
    looping_lcd++;
  }else{
    lcd_kedua();
    looping_lcd = 0;
    
  }

  cek_nodemcu();

  delay(1500);

}

void lcd_pertama() {
  lcd.clear();

  lcd.setCursor(0, 0);
  lcd.print("SU:" + String(s_dht_suhu) + "C");
  lcd.setCursor(8, 0);
  lcd.print("KU:" + String(s_dht_kelembapan) + "%");

  lcd.setCursor(0, 1);
  lcd.print("SA:" + String(s_suhu_air) + "C");
  lcd.setCursor(8, 1);
  lcd.print("VA:" + String(s_volume_air) + "L");

}

void lcd_kedua() {
  lcd.clear();

  lcd.setCursor(0, 0);
  lcd.print("PH :" + String(s_ph_sensor));

  lcd.setCursor(0, 1);
  lcd.print("PPM:" + String(s_tds_sensor));

}

void cek_nodemcu() {
  //baca permintaan nodemcu

  arrData[0] = ""; //Permintaan index 0 = minta, index 1 = kondisi tombol

  String data = "";
  while (DataSerial.available() > 0) {
    data += char(DataSerial.read());
  }

  data.trim();

  if (data != "") {
    //format data suhu_udara#kelembapan_udara#suhu_air#ppm_air#ph_air#volume_air

     int index = 0;
     for (int i = 0; i <= data.length(); i++) {
      char delimiter = '#';
      if (data[i] != delimiter) {
        arrData[index] += data[i];
      } else {
        index++;
      }

    }
    if (arrData[0] == "Minta") {

      //kirim ke nodemcu
      //format data suhu_udara#kelembapan_udara#suhu_air#ppm_air#ph_air#volume_air
      String datakirim = String(s_dht_suhu) + "#" + String(s_dht_kelembapan) + "#" + String(s_suhu_air) + "#" + String(s_tds_sensor) + "#" + String(s_ph_sensor) + "#" + String(s_volume_air)+"#";
      delay(500);
      Serial.println(datakirim);
    }

    

  }
  delay(1000);

  

}

float ph_sensor() {
  for (int i = 0; i < 10; i++)
  {
    buffer_arr[i] = analogRead(A0);
    delay(30);
  }
  for (int i = 0; i < 9; i++)
  {
    for (int j = i + 1; j < 10; j++)
    {
      if (buffer_arr[i] > buffer_arr[j])
      {
        temp = buffer_arr[i];
        buffer_arr[i] = buffer_arr[j];
        buffer_arr[j] = temp;
      }
    }
  }
  avgval = 0;
  for (int i = 2; i < 8; i++){
    avgval += buffer_arr[i];
  }
    
  float volt = (float)avgval * 5.0 / 1024 / 6;
  ph_act = -5.70 * volt + calibration_value;
  return ph_act;

}

float dht_suhu() {
  //Pembacaan dalam format celcius (c)
  float celcius_1 = 0.0;
  celcius_1 = dht.readTemperature();

  return celcius_1;

}

float dht_kelembapan() {
  //Pembacaan untuk data kelembaban
  float humidity_1 = 0.0;
  humidity_1 = dht.readHumidity();

  return humidity_1;
}

float volume_air() {
  digitalWrite(trigPin, HIGH);
  delay (10);
  digitalWrite(trigPin, LOW);
  float data = pulseIn(echoPin, HIGH);
  float jarak = 0.0343 * (data / 2);

  //tinggi air tinggi bak - jarak air

  float tinggi = 100 - jarak;

  //Ukuran panjang x lebar x tinggi dalam cm ke m3 di bagi jt

  float volume = (100 * 100 * tinggi) / 1000000;

  // 1m3 == 1000 liter

  float liter = volume * 1000;

  return liter;
}

float suhu_air() {
  sensorSuhu.requestTemperatures();
  float suhu = sensorSuhu.getTempCByIndex(0);
  return suhu;
}


float tds_sensor() {
  sensorSuhu.requestTemperatures();
  float suhu = sensorSuhu.getTempCByIndex(0);

  digitalWrite(ECPower, HIGH);
  voltage = analogRead(ECPin);
  delay(10);
  digitalWrite(ECPower, LOW);

  Vdrop = (Vref * voltage) / 1024.0;
  Rc = (Vdrop * R1) / (Vref - Vdrop);
  Rc = Rc - Ra;
  EC = (1 / (Rc * K)) * 1000;

  EC25 = EC / (1 + Temp_Coef * (temp1 - 25.0));
  ppm = (EC25) * (ppm_conversion * 1000);

  return ppm;


}

int batas_bawah_suhu = 20;
int batas_atas_suhu = 30;
float batas_bawah_ph = 5.7;
float batas_atas_ph = 6.3;
int batas_bawah_kelembapan = 60;
int batas_atas_kelembapan = 80;

int batas_bawah_pompa_air = 20;
int batas_atas_pompa_air = 35;
int batas_bawah_ph_up = 10;
int batas_atas_ph_up = 25;
int batas_bawah_ph_down = 10;
int batas_atas_ph_down = 25;

float fuzzy_pompa_air(float nilai_kelembapan, float nilai_ph) {
  // R1
  float aPredikat1 = min([kelembapan_kering(nilai_kelembapan), ph_asam(nilai_ph)]);

  // R2
  float aPredikat2 = min([kelembapan_kering(nilai_kelembapan), ph_basa(nilai_ph)]);

  // R3
  float aPredikat3 = min([kelembapan_lembab(nilai_kelembapan), ph_asam(nilai_ph)]);
  
  // R4
  float aPredikat4 = min([kelembapan_lembab(nilai_kelembapan), ph_basa(nilai_ph)]);

  // Inferensi
  // Pompa Air Singkat
  float max_singkat = max([aPredikat1, aPredikat3]);

  // Pompa Air Lama
  float max_lama = max([aPredikat2, aPredikat4]);

  // Titik potong
  float titik_potong_1 = max_singkat * (batas_atas_pompa_air - batas_bawah_pompa_air) + batas_bawah_pompa_air;
  float titik_potong_2 = max_lama * (batas_atas_pompa_air - batas_bawah_pompa_air) + batas_bawah_pompa_air;

  // Defuzzifikasi
  // Momen
  float momen_1 = (titik_potong_1 * titik_potong_1) / (max_singkat * 0.5);
  float momen_2 = (((1 / (batas_atas_pompa_air - batas_bawah_pompa_air) / 3) * (titik_potong_2 * titik_potong_2 * titik_potong_2)) - ((batas_bawah_pompa_air / (batas_atas_pompa_air - batas_bawah_pompa_air) / 2) * (titik_potong_2 * titik_potong_2))) - (((1 / (batas_atas_pompa_air - batas_bawah_pompa_air) / 3) * (titik_potong_1 * titik_potong_1 * titik_potong_1)) - ((batas_bawah_pompa_air / (batas_atas_pompa_air - batas_bawah_pompa_air) / 2) * (titik_potong_1 * titik_potong_1)));
  float momen_3 = ((max_lama / 2) * (batas_atas_pompa_air * batas_atas_pompa_air)) - ((max_lama / 2) * (titik_potong_2 * titik_potong_2));

  //Luas
  float luas_1 = titik_potong_1 * max_singkat;
  float luas_2 = ((1 / (batas_atas_pompa_air - batas_bawah_pompa_air) / 2) * (titik_potong_2 * titik_potong_2) - (batas_bawah_pompa_air * titik_potong_2)) - ((1 / (batas_atas_pompa_air - batas_bawah_pompa_air) / 2) * (titik_potong_1 * titik_potong_1) - (batas_bawah_pompa_air * titik_potong_1));
  float luas_3 = (batas_atas_pompa_air * max_lama) - (titik_potong_2 * max_lama);

  // Hasil
  float hasil = (momen_1 + momen_2 + momen_3) / (luas_1 + luas_2 + luas_3);

  return hasil;
}

float fuzzy_ph_up(float nilai_suhu, float nilai_ph) {
  // R1
  float aPredikat1 = min([suhu_rendah(nilai_suhu), ph_asam(nilai_ph)]);

  // R2
  float aPredikat2 = min([suhu_rendah(nilai_suhu), ph_basa(nilai_ph)]);

  // R3
  float aPredikat3 = min([suhu_tinggi(nilai_suhu), ph_asam(nilai_ph)]);
  
  // R4
  float aPredikat4 = min([suhu_tinggi(nilai_suhu), ph_basa(nilai_ph)]);

  // Inferensi
  //Ph Up Singkat
  float max_singkat = max([aPredikat2, aPredikat4]);

  // Ph Up Lama
  float max_lama = max([aPredikat1, aPredikat3]);

  // Titik potong
  float titik_potong_1 = max_singkat * (batas_atas_ph_up - batas_bawah_ph_up) + batas_bawah_ph_up;
  float titik_potong_2 = max_lama * (batas_atas_ph_up - batas_bawah_ph_up) + batas_bawah_ph_up;

  // Defuzzifikasi
  // Momen
  float momen_1 = (titik_potong_1 * titik_potong_1) / (max_singkat * 0.5);
  float momen_2 = (((1 / (batas_atas_ph_up - batas_bawah_ph_up) / 3) * (titik_potong_2 * titik_potong_2 * titik_potong_2)) - ((batas_bawah_ph_up / (batas_atas_ph_up - batas_bawah_ph_up) / 2) * (titik_potong_2 * titik_potong_2))) - (((1 / (batas_atas_ph_up - batas_bawah_ph_up) / 3) * (titik_potong_1 * titik_potong_1 * titik_potong_1)) - ((batas_bawah_ph_up / (batas_atas_ph_up - batas_bawah_ph_up) / 2) * (titik_potong_1 * titik_potong_1)));
  float momen_3 = ((max_lama / 2) * (batas_atas_ph_up * batas_atas_ph_up)) - ((max_lama / 2) * (titik_potong_2 * titik_potong_2));

  //Luas
  float luas_1 = titik_potong_1 * max_singkat;
  float luas_2 = ((1 / (batas_atas_ph_up - batas_bawah_ph_up) / 2) * (titik_potong_2 * titik_potong_2) - (batas_bawah_ph_up * titik_potong_2)) - ((1 / (batas_atas_ph_up - batas_bawah_ph_up) / 2) * (titik_potong_1 * titik_potong_1) - (batas_bawah_ph_up * titik_potong_1));
  float luas_3 = (batas_atas_ph_up * max_lama) - (titik_potong_2 * max_lama);

  // Hasil
  float hasil = (momen_1 + momen_2 + momen_3) / (luas_1 + luas_2 + luas_3);

  return hasil;
}

float fuzzy_ph_down(float nilai_suhu, float nilai_ph) {
  // R1
  float aPredikat1 = min([suhu_rendah(nilai_suhu), ph_asam(nilai_ph)]);

  // R2
  float aPredikat2 = min([suhu_rendah(nilai_suhu), ph_basa(nilai_ph)]);

  // R3
  float aPredikat3 = min([suhu_tinggi(nilai_suhu), ph_asam(nilai_ph)]);
  
  // R4
  float aPredikat4 = min([suhu_tinggi(nilai_suhu), ph_basa(nilai_ph)]);

  // Inferensi
  //Ph Down Singkat
  float max_singkat = max([aPredikat1, aPredikat3]);

  // Ph Down Lama
  float max_lama = max([aPredikat2, aPredikat4]);

  // Titik potong
  float titik_potong_1 = max_singkat * (batas_atas_ph_down - batas_bawah_ph_down) + batas_bawah_ph_down;
  float titik_potong_2 = max_lama * (batas_atas_ph_down - batas_bawah_ph_down) + batas_bawah_ph_down;

  // Defuzzifikasi
  // Momen
  float momen_1 = (titik_potong_1 * titik_potong_1) / (max_singkat * 0.5);
  float momen_2 = (((1 / (batas_atas_ph_down - batas_bawah_ph_down) / 3) * (titik_potong_2 * titik_potong_2 * titik_potong_2)) - ((batas_bawah_ph_down / (batas_atas_ph_down - batas_bawah_ph_down) / 2) * (titik_potong_2 * titik_potong_2))) - (((1 / (batas_atas_ph_down - batas_bawah_ph_down) / 3) * (titik_potong_1 * titik_potong_1 * titik_potong_1)) - ((batas_bawah_ph_down / (batas_atas_ph_down - batas_bawah_ph_down) / 2) * (titik_potong_1 * titik_potong_1)));
  float momen_3 = ((max_lama / 2) * (batas_atas_ph_down * batas_atas_ph_down)) - ((max_lama / 2) * (titik_potong_2 * titik_potong_2));

  //Luas
  float luas_1 = titik_potong_1 * max_singkat;
  float luas_2 = ((1 / (batas_atas_ph_down - batas_bawah_ph_down) / 2) * (titik_potong_2 * titik_potong_2) - (batas_bawah_ph_down * titik_potong_2)) - ((1 / (batas_atas_ph_down - batas_bawah_ph_down) / 2) * (titik_potong_1 * titik_potong_1) - (batas_bawah_ph_down * titik_potong_1));
  float luas_3 = (batas_atas_ph_down * max_lama) - (titik_potong_2 * max_lama);

  // Hasil
  float hasil = (momen_1 + momen_2 + momen_3) / (luas_1 + luas_2 + luas_3);

  return hasil;
}

float suhu_tinggi(float nilai_suhu) {
  if (nilai_suhu <= batas_bawah_suhu) {
      return 0;
  } else if (nilai_suhu >= batas_bawah_suhu && nilai_suhu <= batas_atas_suhu) {
      return (nilai_suhu - batas_bawah_suhu) / (batas_atas_suhu - batas_bawah_suhu);
  } else if (nilai_suhu >= batas_atas_suhu) {
      return 1;
  }
}

float suhu_rendah(float nilai_suhu) {
  if (nilai_suhu <= batas_bawah_suhu) {
      return 1;
  } else if (nilai_suhu >= batas_bawah_suhu && nilai_suhu <= batas_atas_suhu) {
      return (batas_atas_suhu - nilai_suhu) / (batas_atas_suhu - batas_bawah_suhu);
  } else if (nilai_suhu >= batas_atas_suhu) {
      return 0;
  }
}

float ph_basa(float nilai_ph) {
  if (nilai_ph <= batas_bawah_ph) {
      return 0;
  } else if (nilai_ph >= batas_bawah_ph && nilai_ph <= batas_atas_ph) {
      return (nilai_ph - batas_bawah_ph) / (batas_atas_ph - batas_bawah_ph);
  } else if (nilai_ph >= batas_atas_ph) {
      return 1;
  }
}

float ph_asam(float nilai_ph) {
  if (nilai_ph <= batas_bawah_ph) {
      return 1;
  } else if (nilai_ph >= batas_bawah_ph && nilai_ph <= batas_atas_ph) {
      return (batas_atas_ph - nilai_ph) / (batas_atas_ph - batas_bawah_ph);
  } else if (nilai_ph >= batas_atas_ph) {
      return 0;
  }
}

float kelembapan_lembab(float nilai_kelembapan) {
  if (nilai_kelembapan <= batas_bawah_kelembapan) {
      return 0;
  } else if (nilai_kelembapan >= batas_bawah_kelembapan && nilai_kelembapan <= batas_atas_kelembapan) {
      return (nilai_kelembapan - batas_bawah_kelembapan) / (batas_atas_kelembapan - batas_bawah_kelembapan);
  } else if (nilai_kelembapan >= batas_atas_kelembapan) {
      return 1;
  }
}

float kelembapan_kering(float nilai_kelembapan) {
  if (nilai_kelembapan <= batas_bawah_kelembapan) {
      return 1;
  } else if (nilai_kelembapan >= batas_bawah_kelembapan && nilai_kelembapan <= batas_atas_kelembapan) {
      return (batas_atas_kelembapan - nilai_kelembapan) / (batas_atas_kelembapan - batas_bawah_kelembapan);
  } else if (nilai_kelembapan >= batas_atas_kelembapan) {
      return 0;
  }
}

float pompa_air_lama(float nilai_pompa_air) {
  if (nilai_pompa_air <= batas_bawah_pompa_air) {
      return 0;
  } else if (nilai_pompa_air >= batas_bawah_pompa_air && nilai_pompa_air <= batas_atas_pompa_air) {
      return (nilai_pompa_air - batas_bawah_pompa_air) / (batas_atas_pompa_air - batas_bawah_pompa_air);
  } else if (nilai_pompa_air >= batas_atas_pompa_air) {
      return 1;
  }
}

float pompa_air_singkat(float nilai_pompa_air) {
  if (nilai_pompa_air <= batas_bawah_pompa_air) {
      return 1;
  } else if (nilai_pompa_air >= batas_bawah_pompa_air && nilai_pompa_air <= batas_atas_pompa_air) {
      return (batas_atas_pompa_air - nilai_pompa_air) / (batas_atas_pompa_air - batas_bawah_pompa_air);
  } else if (nilai_pompa_air >= batas_atas_pompa_air) {
      return 0;
  }
}

float ph_up_lama(float nilai_ph_up) {
  if (nilai_ph_up <= batas_bawah_ph_up) {
      return 0;
  } else if (nilai_ph_up >= batas_bawah_ph_up && nilai_ph_up <= batas_atas_ph_up) {
      return (nilai_ph_up - batas_bawah_ph_up) / (batas_atas_ph_up - batas_bawah_ph_up);
  } else if (nilai_ph_up >= batas_atas_ph_up) {
      return 1;
  }
}

float ph_up_singkat(float nilai_ph_up) {
  if (nilai_ph_up <= batas_bawah_ph_up) {
      return 1;
  } else if (nilai_ph_up >= batas_bawah_ph_up && nilai_ph_up <= batas_atas_ph_up) {
      return (batas_atas_ph_up - nilai_ph_up) / (batas_atas_ph_up - batas_bawah_ph_up);
  } else if (nilai_ph_up >= batas_atas_ph_up) {
      return 0;
  }
}

float ph_down_lama(float nilai_ph_down) {
  if (nilai_ph_down <= batas_bawah_ph_down) {
      return 0;
  } else if (nilai_ph_down >= batas_bawah_ph_down && nilai_ph_down <= batas_atas_ph_down) {
      return (nilai_ph_down - batas_bawah_ph_down) / (batas_atas_ph_down - batas_bawah_ph_down);
  } else if (nilai_ph_down >= batas_atas_ph_down) {
      return 1;
  }
}

float ph_down_singkat(float nilai_ph_down) {
  if (nilai_ph_down <= batas_bawah_ph_down) {
      return 1;
  } else if (nilai_ph_down >= batas_bawah_ph_down && nilai_ph_down <= batas_atas_ph_down) {
      return (batas_atas_ph_down - nilai_ph_down) / (batas_atas_ph_down - batas_bawah_ph_down);
  } else if (nilai_ph_down >= batas_atas_ph_down) {
      return 0;
  }
}
