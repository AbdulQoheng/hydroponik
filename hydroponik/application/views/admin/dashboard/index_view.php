    <!-- Main content -->
    <section class="content">
      <div class="container-fluid">
        <!-- Small boxes (Stat box) -->
        <div class="row">
          <div class="col-lg-3 col-6">
              <!-- small box -->
              <div class="small-box bg-warning">
                <div class="inner">
                  <h3><span id="suhu_udara">0</span><sup> C&deg</sup></h3>

                  <p>Suhu Udara</p>
                </div>
                <div class="icon">
                  <i class="far fas fa-temperature-high"></i>
                </div>
                <a href="#" class="small-box-footer">More info <i class="fas fa-arrow-circle-right"></i></a>
              </div>
            </div>
            <!-- ./col -->            
          <div class="col-lg-3 col-6">
            <!-- small box -->
            <div class="small-box bg-info">
              <div class="inner">
                <h3><span id="kelembapan_udara">0</span><sup> %</sup></h3>

                <p>Kelembapan Udara</p>
              </div>
              <div class="icon">
                <i class="far fas fa-snowflake"></i>
              </div>
              <a href="#" class="small-box-footer">More info <i class="fas fa-arrow-circle-right"></i></a>
            </div>
          </div>
          <div class="col-lg-3 col-6">
            <!-- small box -->
            <div class="small-box bg-warning">
              <div class="inner">
                <h3><span id="suhu_air">0</span><sup> C&deg </sup></h3>

                <p>Suhu Air</p>
              </div>
              <div class="icon">
                <i class="far fas fa-thermometer-three-quarters"></i>
              </div>
              <a href="#" class="small-box-footer">More info <i class="fas fa-arrow-circle-right"></i></a>
            </div>
          </div>
          <!-- ./col -->
          <div class="col-lg-3 col-6">
            <!-- small box -->
            <div class="small-box bg-info">
              <div class="inner">
                <h3><span id="ppm_air">0</span><sup> ppm</sup></h3>

                <p>PPM Air</p>
              </div>
              <div class="icon">
                <i class="far fas fa-seedling"></i>
              </div>
              <a href="#" class="small-box-footer">More info <i class="fas fa-arrow-circle-right"></i></a>
            </div>
          </div>
          <!-- ./col -->
          <div class="col-lg-3 col-6">
            <!-- small box -->
            <div class="small-box bg-info">
              <div class="inner">
                <h3><span id="ph_air">0</span><sup> pH</sup></h3>

                <p>PH Air</p>
              </div>
              <div class="icon">
                <i class="far fas fa-leaf"></i>
              </div>
              <a href="#" class="small-box-footer">More info <i class="fas fa-arrow-circle-right"></i></a>
            </div>
          </div>
          <!-- ./col -->
          <div class="col-lg-3 col-6">
            <!-- small box -->
            <div class="small-box bg-warning">
              <div class="inner">
                <h3> <span id="volume_air">0</span> L</h3>

                <p>Volume Air</p>
              </div>
              <div class="icon">
                <i class="far fas fa-tint"></i>
              </div>
              <a href="#" class="small-box-footer">More info <i class="fas fa-arrow-circle-right"></i></a>
            </div>
          </div>
          <!-- ./col -->
        </div>

        <table class="table table-striped table-hover table-sm table-bordered">
        <h3 class="mt-4">Data Pemberian PH UP</h3>
            <thead class="thead-dark">
                <tr>
                    <th>NO.</th>
                    <th>PH</th>
                    <th>TDS</th>
                    <th>Suhu Air</th>
                    <th>Suhu Udara</th>
                    <th>Kelembapan Udara</th>
                    <th>Tanggal</th>
                    <th>Jam</th>
                </tr>
            </thead>
            <tbody>
                <?php 
                  $no = 1;
                ?>
                <?php foreach ($data_phup as $row):

				//membuat variabel $no untuk menyimpan nomor urut
				
				//melakukan perulangan while dengan dari dari query $sql
				?>
                <tr>
                    <td><?php echo ++$no; ?></td>
                    <td><?php echo $row->ph;?></td>
                    <td><?php echo $row->tds;?></td>
                    <td><?php echo $row->suhu_air;?></td>
                    <td><?php echo $row->suhu_udara;?></td>
                    <td><?php echo $row->kelembapan_udara;?></td>
                    <td><?php echo $row->tgl;?></td>
                    <td><?php echo $row->jam;?></td>
                </tr>

                <?php endforeach; ?>
            <tbody>
        </table>

        <table class="table table-striped table-hover table-sm table-bordered">
        <h3 class="mt-4">Data Pemberian PH Down</h3>
            <thead class="thead-dark">
                <tr>
                    <th>NO.</th>
                    <th>PH</th>
                    <th>TDS</th>
                    <th>Suhu Air</th>
                    <th>Suhu Udara</th>
                    <th>Kelembapan Udara</th>
                    <th>Tanggal</th>
                    <th>Jam</th>
                </tr>
            </thead>
            <tbody>
                <?php 
                  $no = 1;
                ?>
                <?php foreach ($data_phdown as $row):

				//membuat variabel $no untuk menyimpan nomor urut
				
				//melakukan perulangan while dengan dari dari query $sql
				?>
                <tr>
                    <td><?php echo ++$no; ?></td>
                    <td><?php echo $row->ph;?></td>
                    <td><?php echo $row->tds;?></td>
                    <td><?php echo $row->suhu_air;?></td>
                    <td><?php echo $row->suhu_udara;?></td>
                    <td><?php echo $row->kelembapan_udara;?></td>
                    <td><?php echo $row->tgl;?></td>
                    <td><?php echo $row->jam;?></td>
                </tr>

                <?php endforeach; ?>
            <tbody>
        </table>

        <table class="table table-striped table-hover table-sm table-bordered">
        <h3 class="mt-4">Data Pemberian AB Mix</h3>
            <thead class="thead-dark">
                <tr>
                    <th>NO.</th>
                    <th>PH</th>
                    <th>TDS</th>
                    <th>Suhu Air</th>
                    <th>Suhu Udara</th>
                    <th>Kelembapan Udara</th>
                    <th>Tanggal</th>
                    <th>Jam</th>
                </tr>
            </thead>
            <tbody>
                <?php 
                  $no = 1;
                ?>
                <?php foreach ($data_abmix as $row):

				//membuat variabel $no untuk menyimpan nomor urut
				
				//melakukan perulangan while dengan dari dari query $sql
				?>
                <tr>
                    <td><?php echo ++$no; ?></td>
                    <td><?php echo $row->ph;?></td>
                    <td><?php echo $row->tds;?></td>
                    <td><?php echo $row->suhu_air;?></td>
                    <td><?php echo $row->suhu_udara;?></td>
                    <td><?php echo $row->kelembapan_udara;?></td>
                    <td><?php echo $row->tgl;?></td>
                    <td><?php echo $row->jam;?></td>
                </tr>

                <?php endforeach; ?>
            <tbody>
        </table>
        <br>
          

          </section>
          <!-- right col -->
        </div>
        <!-- /.row (main row) -->
      </div><!-- /.container-fluid -->
    </section>

    

    <!-- /.content -->
  </div>
  <!-- /.content-wrapper -->

