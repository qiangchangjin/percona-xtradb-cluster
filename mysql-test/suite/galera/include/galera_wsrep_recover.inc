--echo Performing --wsrep-recover ...
#
# When mysqld is run by a root user it will fail to start unless
# we specify what user is ran it (using "--user root" option):
#
if ($MYSQL_TEST_ROOT == 'YES') {
--exec $MYSQLD --defaults-group-suffix=.$galera_wsrep_recover_server_id --defaults-file=$MYSQLTEST_VARDIR/my.cnf --wsrep-recover $galera_wsrep_extra_cmdline_parameter --user root > $MYSQL_TMP_DIR/galera_wsrep_recover.log 2>&1
}
if ($MYSQL_TEST_ROOT != 'YES') {
--exec $MYSQLD --defaults-group-suffix=.$galera_wsrep_recover_server_id --defaults-file=$MYSQLTEST_VARDIR/my.cnf --wsrep-recover $galera_wsrep_extra_cmdline_parameter > $MYSQL_TMP_DIR/galera_wsrep_recover.log 2>&1
}

--perl
	use strict;
        my $wsrep_start_position = `grep 'WSREP: Recovered position:' $ENV{MYSQL_TMP_DIR}/galera_wsrep_recover.log | sed 's/.*WSREP\:\ Recovered\ position://' | sed 's/^[ \t]*//'`;
        chomp($wsrep_start_position);

        die "Could not obtain wsrep_start_position" if $wsrep_start_position eq '';

        open(FILE, ">", "$ENV{MYSQL_TMP_DIR}/galera_wsrep_start_position.inc") or die;
        print FILE "--let \$galera_wsrep_start_position = $wsrep_start_position\n";
        close FILE;
EOF

--source $MYSQL_TMP_DIR/galera_wsrep_start_position.inc

if ($galera_wsrep_start_position == '') {
    --die "Could not obtain wsrep_start_position."
}

--remove_file $MYSQL_TMP_DIR/galera_wsrep_start_position.inc
