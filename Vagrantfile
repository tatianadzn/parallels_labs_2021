Vagrant.configure("2") do |config|
  config.vm.box = "ubuntu/focal64"

  config.vm.provider "virtualbox" do |vb|
    vb.cpus = 4
    vb.memory = 8192
  end

  config.vm.provision "shell", inline: <<-SHELL
    apt-get update
    apt-get install -y gcc cmake

    # install intel c compiler
    # source: https://estuarine.jp/2021/03/install-oneapi/?lang=en
    wget https://apt.repos.intel.com/intel-gpg-keys/GPG-PUB-KEY-INTEL-SW-PRODUCTS.PUB
    apt-key add GPG-PUB-KEY-INTEL-SW-PRODUCTS.PUB
    rm GPG-PUB-KEY-INTEL-SW-PRODUCTS.PUB
    echo "deb https://apt.repos.intel.com/oneapi all main" | sudo tee /etc/apt/sources.list.d/oneAPI.list
    apt-get update
    apt-get install -y intel-basekit intel-hpckit
    echo "source /opt/intel/oneapi/setvars.sh" >> ~/.bashrc

    pushd /tmp
    wget https://fossies.org/linux/misc/old/FW_1.3.1_Lin64.tar.gz
    tar xf FW_1.3.1_Lin64.tar.gz --directory=$HOME
    export FW_HOME=$HOME/FW_1.3.1_Lin64
    export FW_LIB=$FW_HOME/lib
    for file in $(ls -1 $FW_LIB); do ln -sf $FW_LIB/$file $FW_LIB/$(echo $file | sed 's/.1.3.1//'); ln -sf $FW_LIB/$file $FW_LIB/$(echo $file | sed 's/.3.1$//'); done
    export LD_LIBRARY_PATH=$FW_LIB
    popd
  SHELL
end
