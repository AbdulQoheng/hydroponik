<?php
    class Sensor_model extends CI_Model{

        function data_phup() {
            return $this->db->get_where('data', ['kondisi' => 'phup'])->result_array();
        }

        function data_phdown() {
            return $this->db->get_where('data', ['kondisi' => 'phdown'])->result_array();
        }

        function data_abmix() {
            return $this->db->get_where('data', ['kondisi' => 'abmix'])->result_array();
        }

        function sensor() {
            return $this->db->get('sensor');
        }

        function update_sensor() {
            $data = [
                "suhu_udara" => $this->input->get('suhu_udara'),
                "kelembapan_udara" => $this->input->get('kelembapan_udara'),
                "suhu_air" => $this->input->get('suhu_air'),
                "ppm_air" => $this->input->get('ppm_air'),
                "ph_air" => $this->input->get('ph_air'),
                "volume_air" => $this->input->get('volume_air')
            ];

            $this->db->where('id', '1');
            $this->db->update('sensor', $data);
        }
        


    }
?>