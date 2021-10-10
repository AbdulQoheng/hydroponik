<?php
    defined('BASEPATH') OR exit('No direct script access allowed');
    class Api extends CI_Controller {

        function __construct() {
            parent::__construct();
            $this->load->helper('url');
            $this->load->model('sensor_model');
            $this->load->database();
        }

        function suhu_udara() {
            
            $sensor = $this->sensor_model->sensor()->row();
            echo $sensor->suhu_udara;
            
        }

        function kelembapan_udara() {
            
            $sensor = $this->sensor_model->sensor()->row();
            echo $sensor->kelembapan_udara;
            
        }

        function suhu_air() {
            
            $sensor = $this->sensor_model->sensor()->row();
            echo $sensor->suhu_air;
            
        }

        function ppm_air() {
            
            $sensor = $this->sensor_model->sensor()->row();
            echo $sensor->ppm_air;
            
        }

        function ph_air() {
            
            $sensor = $this->sensor_model->sensor()->row();
            echo $sensor->ph_air;
            
        }

        function volume_air() {
            
            $sensor = $this->sensor_model->sensor()->row();
            echo $sensor->volume_air;
            
        }

        function tombol(){
            $sensor = $this->sensor_model->sensor()->row();
            echo $sensor->tombol;
        }

        // http://localhost/hydroponik/api/sensor?password=?hydroponik342&suhu_udara=35&kelembapan_udara=36&suhu_air=37&ppm_air=38&ph_air=39&volume_air=40
        function sensor() {
            if($this->input->get('password')== "hydroponik342"){
                $this->sensor_model->update_sensor();
                $tombol = $this->sensor_model->sensor()->row();
            
                // echo $sensor->tombol;
                echo "Data terkirim";
            }else{
                echo "Data tidak terkirim";
            }
             
            
        }

    }
?>