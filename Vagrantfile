Vagrant.configure("2") do |config|
  config.vm.box = "ubuntu/focal64"

  config.vm.provider "virtualbox" do |vb|
    vb.cpus = 4
    vb.memory = 8192
  end

  config.vm.provision "shell", inline: <<-SHELL
    apt-get update
    apt-get install -y gcc cmake
  SHELL
end
