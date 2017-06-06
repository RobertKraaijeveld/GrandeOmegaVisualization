Rails.application.routes.draw do
  get 'home/index'
  root 'home#index'

  get 'home/gradeavgs', to: 'home#gradeavgs'  
  get 'home/amountofstudentsperclass', to: 'home#amountofstudentsperclass'    
  get 'home/completedexcersises/:percentage', to: 'home#completedexcersises'
  get 'home/kmeans/:percentage', to: 'home#kmeans'      
  get 'home/successrate/:percentage', to: 'home#successrate'
  get 'home/linearregression/:xyvalues', to: 'home#linearregression'
  get 'home/logregression/:xyvalues', to: 'home#logregression'  
  get 'home/correlation/:xyvalues', to: 'home#correlation'  
        
  # For details on the DSL available within this file, see http://guides.rubyonrails.org/routing.html
end
 