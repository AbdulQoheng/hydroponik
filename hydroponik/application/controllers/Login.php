<?php
defined('BASEPATH') OR exit('No direct script access allowed');

class Login extends CI_Controller {

    function __construct(){
        parent::__construct();
        $this->load->model('login_model');
    }
	
	public function index(){
		$this->load->view('login_view');
	}
        
    function aksi() {
        $this->form_validation->set_rules('username','Username','required');
        $this->form_validation->set_rules('password','Password','required');
        
        if ($this->form_validation->run() != false) {
            // menangkap data username dan password dari halaman login
            $username = $this->input->post('username');
            $password = $this->input->post('password');
            $where = array(
            'user_username' => $username,
            'user_password' => md5($password),
            'user_status' => 1
            );
        
            //$this->load->model('m_login');
            // cek kesesuaian login pada table pengguna
            $cek = $this->login_model->cek_login('user',$where);
            //cek login apabila benar
       
            if ($cek->num_rows() > 0) {
                // ambil data pengguna yang melakukan login
                $data = $this->login_model->cek_login('user',$where)->row();
                // buat session untuk pengguna yang berhasil login
                $data_session = array(
                'id' => $data->user_id,
                'name' => $data->user_name,
                'username' => $data->user_username,
                'level' => $data->user_level,
                'status' => 'telah_login'
                );
                $this->session->set_userdata($data_session);
                
                // alihkan halaman ke halaman dashboard pengguna
                if( $this->session->userdata('level') == "admin"){
                    redirect(site_url('admin/dashboard'));
                }else if ($this->session->userdata('level') == "pengunjung"){
                    redirect(site_url('pengunjung/dashboard'));
                }else{
                    redirect('login?alert=gagal');    
                }
        
            }else{
                redirect('login?alert=gagal');
            }
        
        }else{
            $this->load->view('login_view');
        }
        
    }

}