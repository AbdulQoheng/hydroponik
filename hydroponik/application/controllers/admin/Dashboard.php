<?php
    defined('BASEPATH') OR exit('No direct script access allowed');
    class Dashboard extends CI_Controller {

        function __construct() {
            parent::__construct();
            $this->load->helper('url');
            $this->load->model('login_model');
            $this->load->model('user_model');
            $this->load->model('sensor_model');

            /*
            cek session yang login,
            jika session status tidak sama dengan session telah_login, be
            rarti pengguna belum login
            maka halaman akan di alihkan kembali ke halaman login.
            */
            if($this->session->userdata('status') != "telah_login" || $this->session->userdata('level') != "admin"){
                redirect('login?alert=belum_login');
            }
        }
        
        function index() {
            $this->load->database();
            
            $sensor = $this->sensor_model->sensor()->row();

            $data['suhu_udara'] = $sensor->suhu_udara;
            $data['kelembapan_udara'] = $sensor->kelembapan_udara;
            $data['suhu_air'] = $sensor->suhu_air;
            $data['ppm_air'] = $sensor->ppm_air;
            $data['ph_air'] = $sensor->ph_air;
            $data['volume_air'] = $sensor->volume_air;


            $data['data_phup'] = $this->sensor_model->data_phup();
            $data['data_phdown'] = $this->sensor_model->data_phdown();
            $data['data_abmix'] = $this->sensor_model->data_abmix();

            $data['level'] = $this->session->userdata('level');
            $data['user_name'] = $this->session->userdata('name');
            $data['title'] = "Dashboard";
            
            $this->load->view('template/header_view', $data);
            $this->load->view('admin/dashboard/index_view',$data);
            $this->load->view('template/footer_view');
        }

        function keluar() {
            $this->session->sess_destroy();
            redirect('login');
        }

        
    }
?>