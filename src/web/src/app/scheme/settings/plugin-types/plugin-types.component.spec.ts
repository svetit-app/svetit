import { ComponentFixture, TestBed, waitForAsync } from '@angular/core/testing';

import { PluginTypesComponent } from './plugin-types.component';

describe('PluginTypesComponent', () => {
  let component: PluginTypesComponent;
  let fixture: ComponentFixture<PluginTypesComponent>;

  beforeEach(waitForAsync(() => {
    TestBed.configureTestingModule({
      declarations: [ PluginTypesComponent ]
    })
    .compileComponents();
  }));

  beforeEach(() => {
    fixture = TestBed.createComponent(PluginTypesComponent);
    component = fixture.componentInstance;
    fixture.detectChanges();
  });

  it('should create', () => {
    expect(component).toBeTruthy();
  });
});
