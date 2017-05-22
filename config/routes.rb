Rails.application.routes.draw do
  get 'home/index'
  root 'home#index'

  get 'home/gradeavgs', to: 'home#gradeavgs'  
  get 'home/completedexcersises/:percentage', to: 'home#completedexcersises'
  get 'home/kmeans/:percentage', to: 'home#kmeans'      
  get 'home/successrate/:percentage', to: 'home#successrate'
  get 'home/linearregression/:xvalues', to: 'home#linearregression'
        
  # For details on the DSL available within this file, see http://guides.rubyonrails.org/routing.html
end
 