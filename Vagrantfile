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

  SHELL
end
