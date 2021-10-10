<?php
    class User_model extends CI_Model{

        function jumlah_user() {
            return $this->db->get('user')->num_rows();
        }
    }
?>