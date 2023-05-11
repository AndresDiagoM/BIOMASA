<?php
    include "../conexion.php";


    $estado_rele = $_GET['estado_rele'];
    $id_alarma = $_GET['id_alm'];
    echo $estado_rele;
    echo $id_alarma;

    if($estado_rele == 1){
        $sql = "UPDATE datos_medidos SET ESTADO_RELE = 1 WHERE ID_ALARMA = $id_alarma";
        $result = mysqli_query($conectar, $sql);
        echo "<script>
            alert('Se ha ENCENDIDO CORRECTAMENTE el RELE');
            location.href = '../pages/client_alertas.php';
        </script>";
    }
    else{
        $sql2 = "UPDATE datos_medidos SET ESTADO_RELE = 0 WHERE ID_ALARMA = $id_alarma";
        $result2 = mysqli_query($conectar, $sql2);
        echo "<script>
            alert('Se ha APAGADO CORRECTAMENTE el RELE');
            location.href = '../pages/client_alertas.php';
        </script>";
    }




?>